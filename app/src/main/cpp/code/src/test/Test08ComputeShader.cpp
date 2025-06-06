//
// Created by leixing on 2025/1/4.
//

#include "Test08ComputeShader.h"
#include "util/FileUtil.h"

#include <chrono>
#include <thread>
#include <filesystem>
#include <random>
#include <ctime>

namespace test08 {

    Test08ComputeShader::Test08ComputeShader(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        std::vector<const char *> instanceExtensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,

                // old version
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                // new version
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };

        std::vector<const char *> instanceLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::vector<const char *> deviceLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<uint32_t> computeShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/08_compute.comp.spv");
        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/08_compute.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/08_compute.frag.spv");

        uint32_t width = ANativeWindow_getWidth(mApp.window);
        uint32_t height = ANativeWindow_getHeight(mApp.window);

        // Initialize particles
        std::default_random_engine rndEngine((unsigned) time(nullptr));
        std::uniform_real_distribution<float> rndDist(0.0f, 1.0f);

        // Initial particle positions on a circle
        std::vector<Particle> particles(mParticleCount);
        for (auto &particle: particles) {
            float r = 0.25f * sqrt(rndDist(rndEngine));
            float theta = rndDist(rndEngine) * 2.0f * 3.14159265358979323846f;
            float x = r * cos(theta) * height / width;
            float y = r * sin(theta);
            particle.position = glm::vec2(x, y);
            particle.velocity = glm::normalize(glm::vec2(x, y)) * 0.00025f;
            particle.color = glm::vec4(rndDist(rndEngine), rndDist(rndEngine), rndDist(rndEngine), 1.0f);
        }

        VkDeviceSize shaderStorageBufferSize = sizeof(Particle) * mParticleCount;


        mInstance = vklite::InstanceBuilder()
                .extensions(std::move(instanceExtensions))
                .layers(std::move(instanceLayers))
                .buildUnique();
        mSurface = vklite::AndroidSurfaceBuilder()
                .instance(mInstance->getInstance())
                .nativeWindow(mApp.window)
                .buildUnique();
        mPhysicalDevice = std::make_unique<vklite::PhysicalDevice>(vklite::PhysicalDeviceSelector::makeDefault(*mSurface, vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute)
                                                                           .select(mInstance->enumeratePhysicalDevices()).value());

        vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
        if (mMsaaEnable) {
            sampleCount = vklite::MaxMsaaSampleCountSelector(4).select(mPhysicalDevice->querySampleCountFlagBits());
        }
        LOG_D("sampleCount:%d", sampleCount);

        std::vector<uint32_t> presentQueueFamilyIndices = mPhysicalDevice->queryQueueFamilyIndicesBySurface(mSurface->getSurface());
        std::vector<uint32_t> graphicAndComputeQueueFamilyIndices = mPhysicalDevice->queryQueueFamilyIndicesByFlags(vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute);

        mDevice = vklite::DeviceBuilder()
                .physicalDevice(mPhysicalDevice->getPhysicalDevice())
                .extensions(std::move(deviceExtensions))
                .layers(std::move(deviceLayers))
                .addQueueFamily(presentQueueFamilyIndices[0])
                .addQueueFamily(graphicAndComputeQueueFamilyIndices[0])
//                .addDevicePlugin(std::make_unique<vklite::AndroidPlugin>())
                .buildUnique();

        mGraphicQueue = std::make_unique<vklite::Queue>(mDevice->getQueue(graphicAndComputeQueueFamilyIndices[0], 0));
        mPresentQueue = std::make_unique<vklite::Queue>(mDevice->getQueue(presentQueueFamilyIndices[0], 0));
        mComputeQueue = std::make_unique<vklite::Queue>(mDevice->getQueue(graphicAndComputeQueueFamilyIndices[0], 0));

        mSwapchain = vklite::SwapchainBuilder()
                .device(mDevice->getDevice())
                .surface(mSurface->getSurface())
                .queueFamilyIndices({presentQueueFamilyIndices[0]})
                .config(mPhysicalDevice->getPhysicalDevice(), mSurface->getSurface())
                .buildUnique();

        LOG_D("mSwapchain->getDisplaySize(): [%d x %d]", mSwapchain->getDisplaySize().width, mSwapchain->getDisplaySize().height);
        vk::Viewport viewport;
        viewport
                .setX(0.0f)
                .setY(0.0f)
                .setWidth((float) mSwapchain->getDisplaySize().width)
                .setHeight((float) mSwapchain->getDisplaySize().height)
                .setMinDepth(0.0f)
                .setMaxDepth(1.0f);
        mViewports.push_back(viewport);

        vk::Rect2D scissor{};
        scissor
                .setOffset(vk::Offset2D{0, 0})
                .setExtent(mSwapchain->getDisplaySize());
        mScissors.push_back(scissor);


        mCommandPool = vklite::CommandPoolBuilder()
                .device(mDevice->getDevice())
                .queueFamilyIndex(graphicAndComputeQueueFamilyIndices[0])
                .buildUnique();
        mCommandBuffers = mCommandPool->allocateUnique(mFrameCount);

        // 创建附件
        mDisplayImageViews = mSwapchain->createDisplayImageViews();

        if (mMsaaEnable) {
            mColorImageView = vklite::CombinedImageViewBuilder().asColor()
                    .device(mDevice->getDevice())
                    .format(mSwapchain->getDisplayFormat())
                    .size(mSwapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .configDeviceMemory(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
                    .buildUnique();
        }

        if (mDepthTestEnable) {
            mDepthImageView = vklite::CombinedImageViewBuilder().asDepth()
                    .device(mDevice->getDevice())
                    .format(mPhysicalDevice->findDepthFormat())
                    .size(mSwapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .configDeviceMemory(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
                    .buildUnique();
        }

        vklite::Subpass externalSubpass = vklite::Subpass::externalSubpass();
        mRenderPass = vklite::RenderPassBuilder()
                .device(mDevice->getDevice())
                .renderAreaExtend(mSwapchain->getDisplaySize())
                .addSubpass([&](vklite::Subpass &Subpass, const std::vector<vklite::Subpass> &subpasses) {
                    Subpass
                            .pipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                            .stageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
                            .accessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
                            .addDependency(externalSubpass);
                })
                .addAttachmentIf(mMsaaEnable, [&](vklite::Attachment &attachment, std::vector<vklite::Subpass> &subpasses) {
                    vklite::Attachment::msaaColorAttachment(attachment)
                            .sampleCount(sampleCount)
                            .format(mSwapchain->getDisplayFormat())
                            .clearColorValue(mClearColor)
                            .asColorAttachmentUsedIn(subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                })
                .addAttachment([&](vklite::Attachment &attachment, std::vector<vklite::Subpass> &subpasses) {
                    vklite::Attachment::presentColorAttachment(attachment)
                            .format(mSwapchain->getDisplayFormat())
                            .clearColorValue(mClearColor)
                            .applyIf(mMsaaEnable, [&](vklite::Attachment &thiz) {
                                thiz
                                        .loadOp(vk::AttachmentLoadOp::eDontCare)
                                        .asResolveAttachmentUsedIn(subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                            })
                            .applyIf(!mMsaaEnable, [&](vklite::Attachment &thiz) {
                                thiz.asColorAttachmentUsedIn(subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                            });
                })
                .addAttachmentIf(mDepthTestEnable, [&](vklite::Attachment &attachment, std::vector<vklite::Subpass> &subpasses) {
                    vklite::Attachment::depthStencilAttachment(attachment)
                            .sampleCount(sampleCount)
                            .clearDepthValue(mClearDepth)
                            .format(mPhysicalDevice->findDepthFormat())
                            .asDepthStencilAttachmentUsedIn(subpasses[0], vk::ImageLayout::eDepthStencilAttachmentOptimal);
                })
                .buildUnique();

        mFramebuffers = vklite::FramebuffersBuilder()
                .count(mDisplayImageViews.size())
                .framebufferBuilder([&](uint32_t index) {
                    return vklite::FramebufferBuilder()
                            .device(mDevice->getDevice())
                            .renderPass(mRenderPass->getRenderPass())
                            .width(mSwapchain->getDisplaySize().width)
                            .height(mSwapchain->getDisplaySize().height)
                                    // 下面添加附件的顺序不能乱, 附件的顺序由 RenderPass 的附件定义顺序决定，必须严格一致。
                            .addAttachmentIf(mMsaaEnable, [&]() { return mColorImageView->getImageView().getImageView(); })
                            .addAttachment(mDisplayImageViews[index].getImageView())
                            .addAttachmentIf(mDepthTestEnable, [&]() { return mDepthImageView->getImageView().getImageView(); })
                            .build();
                })
                .build();

        mImageAvailableSemaphores = vklite::SemaphoreBuilder().device(mDevice->getDevice()).build(mFrameCount);
        mRenderFinishedSemaphores = vklite::SemaphoreBuilder().device(mDevice->getDevice()).build(mFrameCount);
        mFences = vklite::FenceBuilder()
                .device(mDevice->getDevice())
                        // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
                .fenceCreateFlags(vk::FenceCreateFlagBits::eSignaled)
                .build(mFrameCount);


        vklite::ShaderConfigure graphicShaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Particle))
//                            .inputRate(vk::VertexInputRate::eVertex)
                            .addAttribute(0, ShaderFormat::Vec2, offsetof(Particle, position))
                            .addAttribute(1, ShaderFormat::Vec4, offsetof(Particle, color));
                });

        mGraphicsDescriptorPool = vklite::DescriptorPoolBuilder()
                .device(mDevice->getDevice())
                .frameCount(mFrameCount)
                .buildUnique();

        mGraphicsPipelineLayout = vklite::PipelineLayoutBuilder()
                .device(mDevice->getDevice())
                .buildUnique();

        mGraphicsPipeline = vklite::GraphicsPipelineBuilder()
                .device(mDevice->getDevice())
                .renderPass(mRenderPass->getRenderPass())
                .pipelineLayout(mGraphicsPipelineLayout->getPipelineLayout())
                .viewports(mViewports)
                .scissors(mScissors)
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .buildUnique();

        // compute pipeline
        mComputeCommandBuffers = mCommandPool->allocateUnique(mFrameCount);

        vklite::ShaderConfigure computeShaderConfigure = vklite::ShaderConfigure()
                .computeShaderCode(std::move(computeShaderCode))
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniform(0, 1, vk::ShaderStageFlagBits::eCompute)
                            .addStorage(1, 1, vk::ShaderStageFlagBits::eCompute)
                            .addStorage(2, 1, vk::ShaderStageFlagBits::eCompute);
                });

        mComputeDescriptorPool = vklite::DescriptorPoolBuilder()
                .device(mDevice->getDevice())
                .config(computeShaderConfigure)
                .frameCount(mFrameCount)
                .buildUnique();

        mComputePipelineLayout = vklite::PipelineLayoutBuilder()
                .device(mDevice->getDevice())
//                .config(computeShaderConfigure)
                .buildUnique();

        mComputePipeline = vklite::ComputePipelineBuilder()
                .device(mDevice->getDevice())
                .pipelineLayout(mComputePipelineLayout->getPipelineLayout())
                .buildUnique();

        mComputePipelineResources = vklite::PipelineResourcesBuilder()
                .frameCount(mFrameCount)
//                .pipelineResourceBuilder([&](uint32_t frameIndex) {
//                    return vklite::PipelineResourceBuilder()
//                            .descriptorSets(mComputeDescriptorPool->allocateDescriptorSets(mComputePipelineLayout->getDescriptorSetLayouts()))
//                            .build();
//                })
                .build();

        for (int i = 0; i < mFrameCount; i++) {
            vklite::StorageBuffer storageBuffer = vklite::StorageBufferBuilder()
                    .device(mDevice->getDevice())
                    .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
                    .size(shaderStorageBufferSize)
                    .addUsage(vk::BufferUsageFlagBits::eVertexBuffer)
                    .build();
            storageBuffer.update(*mCommandPool, particles.data(), shaderStorageBufferSize);
            mShaderStorageBuffers.push_back(std::move(storageBuffer));

            vklite::UniformBuffer uniformBuffer = vklite::UniformBufferBuilder()
                    .device(mDevice->getDevice())
//                    .phys(mPhysicalDevice->getPhysicalDevice())
                    .size(sizeof(UniformBufferObject))
                    .build();

            mUniformBuffers.push_back(std::move(uniformBuffer));
        }

        mComputeFences = vklite::FenceBuilder()
                .device(mDevice->getDevice())
                        // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
                .fenceCreateFlags(vk::FenceCreateFlagBits::eSignaled)
                .build(mFrameCount);
        mComputeFinishSemaphores = vklite::SemaphoreBuilder()
                .device(mDevice->getDevice())
                .build(mFrameCount);

//        vklite::DescriptorSetWriter computePipelineDescriptorSetWriter = vklite::DescriptorSetWriterBuilder()
//                .frameCount(mFrameCount)
//                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &descriptorSetMappingConfigure) {
//                    descriptorSetMappingConfigure
//                            .descriptorSet(mComputePipelineResources[frameIndex].getDescriptorSets()[0])
//                            .addMapping([&](vklite::DescriptorMapping &mapping) {
//                                mapping
//                                        .binding(0)
//                                        .descriptorType(vk::DescriptorType::eUniformBuffer)
//                                        .addBufferInfo(mUniformBuffers[frameIndex].getCombinedMemoryBuffer().getBuffer());
//                            })
//                            .addMapping([&](vklite::DescriptorMapping &mapping) {
//                                mapping
//                                        .binding(1)
//                                        .descriptorType(vk::DescriptorType::eStorageBuffer)
//                                        .addBufferInfo(mShaderStorageBuffers[(frameIndex - 1) % mFrameCount].getVkBuffer(), 0, (vk::DeviceSize) shaderStorageBufferSize);
//                            })
//                            .addMapping([&](vklite::DescriptorMapping &mapping) {
//                                mapping
//                                        .binding(2)
//                                        .descriptorType(vk::DescriptorType::eStorageBuffer)
//                                        .addBufferInfo(mShaderStorageBuffers[frameIndex].getVkBuffer(), 0, (vk::DeviceSize) shaderStorageBufferSize);
//                            });
//                })
//                .build();
//        mDevice->getDevice().updateDescriptorSets(computePipelineDescriptorSetWriter.createWriteDescriptorSets(), nullptr);

        mTimer.start();
        LOG_D("test created ");
    }

    void Test08ComputeShader::init() {

    }

    // 检查是否准备好
    bool Test08ComputeShader::isReady() {
        return true;
    }

    // 绘制帧
    void Test08ComputeShader::drawFrame() {
        // update data by frame
        UniformBufferObject ubo{};
        ubo.deltaTime = static_cast<float >(mTimer.getElapsedTimeMs()) * 2.0f;
        mUniformBuffers[mCurrentFrameIndex].update(*mCommandPool, &ubo, sizeof(UniformBufferObject));

        const vk::Device vkDevice = mDevice->getDevice();

        // compute pipeline
        vk::Result result = mComputeFences[mCurrentFrameIndex].wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        result = mComputeFences[mCurrentFrameIndex].reset();
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("mComputeFences[mCurrentFrameIndex] resetFences failed");
        }

        const vklite::PooledCommandBuffer &computeCommandBuffer = (*mComputeCommandBuffers)[mCurrentFrameIndex];
        computeCommandBuffer.record([&](const vk::CommandBuffer &commandBuffer) {
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, mComputePipeline->getPipeline());
            const std::vector<vk::DescriptorSet> &descriptorSets = mComputePipelineResources[mCurrentFrameIndex].getDescriptorSets();
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, mComputePipelineLayout->getPipelineLayout(), 0, descriptorSets, nullptr);

            commandBuffer.dispatch(mParticleCount / 256, 1, 1);
        });

        mComputeQueue->submit(computeCommandBuffer.getCommandBuffer(),
                              mComputeFinishSemaphores[mCurrentFrameIndex].getSemaphore(),
                              mComputeFences[mCurrentFrameIndex].getFence());

        result = mComputeFences[mCurrentFrameIndex].wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // graphic pipeline
        // GPU-GPU 同步（跨队列或同一队列不同提交批次）
        vklite::Semaphore &imageAvailableSemaphore = mImageAvailableSemaphores[mCurrentFrameIndex];
        vklite::Semaphore &renderFinishedSemaphore = mRenderFinishedSemaphores[mCurrentFrameIndex];
        // Fence主要用于CPU和GPU之间的同步，比如让CPU等待GPU完成某个任务
        vklite::Fence &fence = mFences[mCurrentFrameIndex];
        result = fence.wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
        // while(gpu.imageAvailableSemaphore != 1){/* wait */}
        // imageIndex = acquireNextImage();
        auto [acquireResult, imageIndex] = mSwapchain->acquireNextImage(imageAvailableSemaphore.getSemaphore());
        if (acquireResult != vk::Result::eSuccess) {
            if (acquireResult == vk::Result::eErrorOutOfDateKHR) {
                // 交换链已与表面不兼容，不能再用于渲染。通常在窗口大小调整后发生。
                LOG_E("acquireNextImageKHR: eErrorOutOfDateKHR, recreateSwapChain");
//                recreateSwapChain();
                return;
            } else if (acquireResult == vk::Result::eSuboptimalKHR) {
                //vk::Result::eSuboptimalKHR 交换链仍然可以成功显示到表面，但表面属性不再完全匹配。
                LOG_D("acquireNextImageKHR: eSuboptimalKHR");
            } else {
                LOG_E("acquireNextImageKHR: failed: %d", acquireResult);
                throw std::runtime_error("acquireNextImageKHR failed");
            }
        }

        const vklite::PooledCommandBuffer &commandBuffer = (*mCommandBuffers)[mCurrentFrameIndex];
        commandBuffer.record([&](const vk::CommandBuffer &vkCommandBuffer) {
            mRenderPass->execute(vkCommandBuffer, mFramebuffers[imageIndex].getFramebuffer(), [&](const vk::CommandBuffer &vkCommandBuffer) {
                vkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mGraphicsPipeline->getPipeline());
                vkCommandBuffer.setViewport(0, mViewports);
                vkCommandBuffer.setScissor(0, mScissors);
                std::array<vk::DeviceSize, 1> offsets = {0};
                std::array<vk::Buffer, 1> vertexBuffers = {mShaderStorageBuffers[mCurrentFrameIndex].getVkBuffer()};
                vkCommandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);
                vkCommandBuffer.draw(mParticleCount, 1, 0, 0);
            });
        });

        result = mFences[mCurrentFrameIndex].reset();
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("resetFences failed");
        }

        // while(gpu.imageAvailableSemaphore != 1){ /* wait */ }
        // render();
        // gpu.renderFinishedSemaphore = 1;
        // cpu.fence = 1;
        mGraphicQueue->submit(commandBuffer.getCommandBuffer(), vk::PipelineStageFlagBits::eColorAttachmentOutput,
                              imageAvailableSemaphore.getSemaphore(), renderFinishedSemaphore.getSemaphore(), fence.getFence());

        //while(gpu.renderFinishedSemaphore != 1){ /* wait */ }
        // present()
        // gpu.imageAvailableSemaphore = 1;
        result = mPresentQueue->present(mSwapchain->getSwapChain(), imageIndex, renderFinishedSemaphore.getSemaphore());
        if (result != vk::Result::eSuccess) {
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mFramebufferResized) {
                mFramebufferResized = false;
                LOG_E("presentKHR: eErrorOutOfDateKHR or eSuboptimalKHR or mFramebufferResized, recreateSwapChain");
                // todo: recreateSwapChain
//                recreateSwapChain();
                return;
            } else {
                throw std::runtime_error("presentKHR failed");
            }
        }

        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mFrameCount;
    }

    // 清理操作
    void Test08ComputeShader::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }
} // test
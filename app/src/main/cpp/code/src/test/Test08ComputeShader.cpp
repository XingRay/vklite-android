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

        std::vector<std::string> instanceExtensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,

                // old version
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                // new version
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };

        std::vector<std::string> instanceLayers = {
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
                .extensions({}, std::move(instanceExtensions))
                .layers({}, std::move(instanceLayers))
                .build();
        mSurface = vklite::AndroidSurfaceBuilder(mApp.window).build(*mInstance);
        mPhysicalDevice = vklite::PhysicalDeviceSelector::makeDefault(*mSurface)->select(mInstance->listPhysicalDevices());

        vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
        if (mMsaaEnable) {
            sampleCount = mPhysicalDevice->selectMaxMsaaSampleCountFlagBits(4);
        }
        LOG_D("sampleCount:%d", sampleCount);
        vklite::QueueFamilyIndices queueFamilyIndices = mPhysicalDevice->queryQueueFamilies(mSurface->getSurface(), vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute);

        mDevice = vklite::DeviceBuilder()
                .physicalDevice(mPhysicalDevice->getPhysicalDevice())
                .extensions(std::move(deviceExtensions))
                .layers(std::move(deviceLayers))
                .addQueueFamily(queueFamilyIndices.graphicQueueFamilyIndex.value())
                .addQueueFamily(queueFamilyIndices.graphicQueueFamilyIndex.value())
                .addQueueFamily(queueFamilyIndices.presentQueueFamilyIndex.value())
//                .addDevicePlugin(std::make_unique<vklite::AndroidPlugin>())
                .buildUnique();

        mSwapchain = vklite::SwapchainBuilder()
                .build(*mPhysicalDevice, *mDevice, *mSurface, {queueFamilyIndices.presentQueueFamilyIndex.value()});

        mCommandPool = vklite::CommandPoolBuilder()
                .queueFamilyIndex(queueFamilyIndices.graphicQueueFamilyIndex.value())
                .build(*mDevice);
        mCommandBuffers = mCommandPool->allocateUnique(mFrameCount);

        // 创建附件
        mDisplayImageViews = vklite::ImageViewBuilder::colorImageViewBuilder()
                .format(mSwapchain->getDisplayFormat())
                .build(*mDevice, mSwapchain->getDisplayImages());

        if (mMsaaEnable) {
            mColorImage = vklite::ImageBuilder::colorImageBuilder()
                    .width(mSwapchain->getDisplaySize().width)
                    .height(mSwapchain->getDisplaySize().height)
                    .format(mSwapchain->getDisplayFormat())
                    .sampleCount(sampleCount)
                    .buildUnique(*mPhysicalDevice, *mDevice);
            mColorImageView = vklite::ImageViewBuilder::colorImageViewBuilder()
                    .format(mSwapchain->getDisplayFormat())
                    .buildUnique(*mDevice, *mColorImage);
        }

        if (mDepthTestEnable) {
            vk::Format depthFormat = mPhysicalDevice->findDepthFormat();

            std::unique_ptr<vklite::Image> depthImage = vklite::ImageBuilder::depthImageBuilder()
                    .width(mSwapchain->getDisplaySize().width)
                    .height(mSwapchain->getDisplaySize().height)
                    .format(depthFormat)
                    .sampleCount(sampleCount)
                    .buildUnique(*mPhysicalDevice, *mDevice);
            depthImage->transitionImageLayout(*mCommandPool, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1,
                                              vk::QueueFamilyIgnored, vk::QueueFamilyIgnored, vk::ImageAspectFlagBits::eDepth);
            mDepthImage = std::move(depthImage);

            mDepthImageView = vklite::ImageViewBuilder::depthImageViewBuilder()
                    .format(depthFormat)
                    .buildUnique(*mDevice, *mDepthImage);
        }

        vklite::Subpass externalSubpass = vklite::Subpass::externalSubpass();
        mRenderPass = vklite::RenderPassBuilder()
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
//                            .asResolveAttachmentUsedInIf(mMsaaEnable, subpasses[0], vk::ImageLayout::eColorAttachmentOptimal)
//                            .asColorAttachmentUsedInIf(!mMsaaEnable, subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                })
                .addAttachmentIf(mDepthTestEnable, [&](vklite::Attachment &attachment, std::vector<vklite::Subpass> &subpasses) {
                    vklite::Attachment::depthStencilAttachment(attachment)
                            .sampleCount(sampleCount)
                            .clearDepthValue(mClearDepth)
                            .format(mPhysicalDevice->findDepthFormat())
                            .asDepthStencilAttachmentUsedIn(subpasses[0], vk::ImageLayout::eDepthStencilAttachmentOptimal);
                })
                .renderAreaExtend(mSwapchain->getDisplaySize())
                .buildUnique(*mDevice);

        mFrameBuffers = vklite::FrameBuffersBuilder()
                .count(mDisplayImageViews.size())
                .frameBufferBuilder([&](uint32_t index) {
                    return vklite::FrameBufferBuilder()
                            .width(mSwapchain->getDisplaySize().width)
                            .height(mSwapchain->getDisplaySize().height)
                                    // 下面添加附件的顺序不能乱, 附件的顺序由 RenderPass 的附件定义顺序决定，必须严格一致。
                            .addAttachmentIf(mMsaaEnable, [&]() { return mColorImageView->getImageView(); })
                            .addAttachment(mDisplayImageViews[index].getImageView())
                            .addAttachmentIf(mDepthTestEnable, [&]() { return mDepthImageView->getImageView(); })
                            .build(*mDevice, *mRenderPass);
                })
                .build();

        mSyncObject = vklite::SyncObjectBuilder()
                .frameCount(mFrameCount)
                .build(*mDevice);

//        vklite::DescriptorConfigure graphicsDescriptorConfigure = vklite::DescriptorConfigure()
//                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
//                    descriptorSetConfigure
//                            .set(0)
//                            .addSampler([](vklite::SamplerConfigure &samplerConfigure) {
//                                samplerConfigure
//                                        .binding(0)
//                                        .shaderStageFlags(vk::ShaderStageFlagBits::eFragment);
//                            });
//                });

        mGraphicsDescriptorPool = vklite::DescriptorPoolBuilder()
//                .descriptorPoolSizes(graphicsDescriptorConfigure.calcDescriptorPoolSizes())
//                .descriptorSetCount(graphicsDescriptorConfigure.getDescriptorSetCount())
                .frameCount(mFrameCount)
                .buildUnique(*mDevice);

        std::vector<vk::DescriptorSetLayout> graphicsDescriptorSetLayouts;// = graphicsDescriptorConfigure.createDescriptorSetLayouts(*mDevice);

        mGraphicsPipelineLayout = vklite::PipelineLayoutBuilder()
                .addPushConstant(sizeof(glm::mat4), 0, vk::ShaderStageFlagBits::eVertex)
                .buildUnique(*mDevice, graphicsDescriptorSetLayouts);

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

        mGraphicsPipeline = vklite::GraphicsPipelineBuilder()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Particle))
//                            .inputRate(vk::VertexInputRate::eVertex)
                            .addAttribute(0, ShaderFormat::Vec2, offsetof(Particle, position))
                            .addAttribute(1, ShaderFormat::Vec4, offsetof(Particle, color));
                })
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .buildUnique(*mDevice, *mRenderPass, *mGraphicsPipelineLayout, mViewports, mScissors);

//        mIndexBuffer = vklite::IndexBufferBuilder()
//                .bufferSize(indices.size() * sizeof(uint32_t))
//                .build(*mDevice);
//        mIndexBuffer->update(*mCommandPool, indices);

//        mVertexBuffer = vklite::VertexBufferBuilder()
//                .bufferSize(vertices.size() * sizeof(Vertex))
//                .build(*mDevice);
//        mVertexBuffer->update(*mCommandPool, vertices);

//        mGraphicsPipelineResources = vklite::PipelineResourcesBuilder()
//                .frameCount(mFrameCount)
//                .pipelineResourceBuilder([&](uint32_t frameIndex) {
//                    return vklite::PipelineResourceBuilder()
//                            .addVertexBuffer(*mVertexBuffer)
//                            .indexBuffer(*mIndexBuffer)
//                            .indicesCount(indices.size())
//                            .descriptorSets(mGraphicsDescriptorPool->allocateDescriptorSets(graphicsDescriptorSetLayouts))
//                            .build();
//                })
//                .build();

//        vklite::DescriptorSetWriter descriptorSetWriter = vklite::DescriptorSetWriterBuilder()
//                .frameCount(mFrameCount)
//                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &descriptorSetMappingConfigure) {
//                    descriptorSetMappingConfigure
//                            .addMapping([&](vklite::DescriptorMapping &mapping) {
//                                mapping
//                                        .descriptorSet(mPipelineResources[frameIndex].getDescriptorSets()[0])
//                                        .binding(0)
//                                        .descriptorType(vk::DescriptorType::eCombinedImageSampler);
////                                        .addImageInfo(*mSamplers[frameIndex], *mImageViews[frameIndex]);
//                            });
//                })
//                .build();
//        mDevice->getDevice().updateDescriptorSets(descriptorSetWriter.createWriteDescriptorSets(), nullptr);

        // compute pipeline
        mComputeCommandBuffers = mCommandPool->allocateUnique(mFrameCount);

        vklite::DescriptorConfigure computeDescriptorConfigure = vklite::DescriptorConfigure()
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniform(0, 1, vk::ShaderStageFlagBits::eCompute)
                            .addStorage(1, 1, vk::ShaderStageFlagBits::eCompute)
                            .addStorage(2, 1, vk::ShaderStageFlagBits::eCompute);
                });

        mComputeDescriptorPool = vklite::DescriptorPoolBuilder()
                .descriptorPoolSizes(computeDescriptorConfigure.calcDescriptorPoolSizes())
                .descriptorSetCount(computeDescriptorConfigure.getDescriptorSetCount())
                .frameCount(mFrameCount)
                .buildUnique(*mDevice);

        std::vector<vk::DescriptorSetLayout> computeDescriptorSetLayouts = computeDescriptorConfigure.createDescriptorSetLayouts(*mDevice);

        mComputePipelineLayout = vklite::PipelineLayoutBuilder()
                .buildUnique(*mDevice, computeDescriptorSetLayouts);

        mComputePipeline = vklite::ComputePipelineBuilder()
                .device(mDevice->getDevice())
                .pipelineLayout(mComputePipelineLayout->getPipelineLayout())
                .computeShaderCode(std::move(computeShaderCode))
                .buildUnique();

        mComputePipelineResources = vklite::PipelineResourcesBuilder()
                .frameCount(mFrameCount)
                .pipelineResourceBuilder([&](uint32_t frameIndex) {
                    return vklite::PipelineResourceBuilder()
                            .descriptorSets(mComputeDescriptorPool->allocateDescriptorSets(computeDescriptorSetLayouts))
                            .build();
                })
                .build();

        vklite::StagingBuffer stagingBuffer(*mPhysicalDevice, *mDevice, shaderStorageBufferSize);
        stagingBuffer.updateBuffer(particles.data(), shaderStorageBufferSize);
        for (int i = 0; i < mFrameCount; i++) {
            vklite::DeviceLocalBuffer deviceLocalBuffer(*mPhysicalDevice, *mDevice, shaderStorageBufferSize, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer);
            mCommandPool->submitOneTimeCommand([&](const vk::CommandBuffer commandBuffer) {
                deviceLocalBuffer.recordCommandCopyFrom(commandBuffer, stagingBuffer.getBuffer());
            });
            mShaderStorageBuffers.push_back(std::move(deviceLocalBuffer));

            std::unique_ptr<vklite::UniformBuffer> uniformBuffer = std::make_unique<vklite::UniformBuffer>(*mPhysicalDevice, *mDevice, sizeof(UniformBufferObject));
            mUniformBuffers.push_back(std::move(uniformBuffer));

            vk::SemaphoreCreateInfo semaphoreCreateInfo{};
            vk::FenceCreateInfo fenceCreateInfo{};
            // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
            fenceCreateInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

            vk::Device vkDevice = (*mDevice).getDevice();
            mComputeFences.push_back(vkDevice.createFence(fenceCreateInfo));
            mComputeFinishSemaphores.push_back(vkDevice.createSemaphore(semaphoreCreateInfo));
        }

        vklite::DescriptorSetWriter computePipelineDescriptorSetWriter = vklite::DescriptorSetWriterBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &descriptorSetMappingConfigure) {
                    descriptorSetMappingConfigure
                            .addMapping([&](vklite::DescriptorMapping &mapping) {
                                mapping
                                        .descriptorSet(mComputePipelineResources[frameIndex].getDescriptorSets()[0])
                                        .binding(0)
                                        .descriptorType(vk::DescriptorType::eUniformBuffer)
                                        .addBufferInfo(*mUniformBuffers[frameIndex]);
                            })
                            .addMapping([&](vklite::DescriptorMapping &mapping) {
                                mapping
                                        .descriptorSet(mComputePipelineResources[frameIndex].getDescriptorSets()[0])
                                        .binding(1)
                                        .descriptorType(vk::DescriptorType::eStorageBuffer)
                                        .addBufferInfo(mShaderStorageBuffers[(frameIndex - 1) % mFrameCount].getBuffer(), 0, (vk::DeviceSize) shaderStorageBufferSize);
                            })
                            .addMapping([&](vklite::DescriptorMapping &mapping) {
                                mapping
                                        .descriptorSet(mComputePipelineResources[frameIndex].getDescriptorSets()[0])
                                        .binding(2)
                                        .descriptorType(vk::DescriptorType::eStorageBuffer)
                                        .addBufferInfo(mShaderStorageBuffers[frameIndex].getBuffer(), 0, (vk::DeviceSize) shaderStorageBufferSize);
                            });
                })
                .build();
        mDevice->getDevice().updateDescriptorSets(computePipelineDescriptorSetWriter.createWriteDescriptorSets(), nullptr);

        mTimer.start();
        LOG_D("test created ");
    }

    void Test08ComputeShader::init() {

    }

    // 检查是否准备好
    bool Test08ComputeShader::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test08ComputeShader::drawFrame() {
        const vk::Device vkDevice = mDevice->getDevice();

        // compute pipeline
        vk::Result result = vkDevice.waitForFences(mComputeFences[mCurrentFrameIndex], vk::True, std::numeric_limits<uint64_t>::max());
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        UniformBufferObject ubo{};
        ubo.deltaTime = static_cast<double>(mTimer.getDeltaTimeMs()) * 2.0f;
        mUniformBuffers[mCurrentFrameIndex]->update(*mCommandPool, &ubo, sizeof(UniformBufferObject));

        result = vkDevice.resetFences(1, &mComputeFences[mCurrentFrameIndex]);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("mComputeFences[mCurrentFrameIndex] resetFences failed");
        }

        const vklite::CommandBuffer &computeCommandBuffer = (*mComputeCommandBuffers)[mCurrentFrameIndex];
        computeCommandBuffer.execute([&](const vk::CommandBuffer &commandBuffer) {
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, mComputePipeline->getPipeline());
            const std::vector<vk::DescriptorSet> &descriptorSets = mComputePipelineResources[mCurrentFrameIndex].getDescriptorSets();
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, mComputePipelineLayout->getPipelineLayout(), 0, descriptorSets, nullptr);

            vkCmdDispatch(commandBuffer, mParticleCount / 256, 1, 1);
        });

        std::array<vk::CommandBuffer, 1> computeCommandBuffers = {computeCommandBuffer.getCommandBuffer()};
        std::array<vk::Semaphore, 1> computeSignalSemaphores = {mComputeFinishSemaphores[mCurrentFrameIndex]};

        vk::SubmitInfo computeSubmitInfo{};
        computeSubmitInfo
                .setCommandBuffers(computeCommandBuffers)
                .setSignalSemaphores(computeSignalSemaphores);

        std::array<vk::SubmitInfo, 1> computeSubmitInfos = {computeSubmitInfo};
        //todo:
//        mDevice->getComputeQueue().submit(computeSubmitInfos, mComputeFences[mCurrentFrameIndex]);
        std::array<vk::Fence, 1> computeWaitFences = {mComputeFences[mCurrentFrameIndex]};
        result = vkDevice.waitForFences(computeWaitFences, vk::True, std::numeric_limits<uint64_t>::max());
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // graphic pipeline
        vk::Semaphore imageAvailableSemaphore = mSyncObject->getImageAvailableSemaphore(mCurrentFrameIndex);
        vk::Semaphore renderFinishedSemaphore = mSyncObject->getRenderFinishedSemaphore(mCurrentFrameIndex);
        vk::Fence fence = mSyncObject->getFence(mCurrentFrameIndex);
        std::array<vk::Fence, 1> waitFences = {fence};

        result = vkDevice.waitForFences(waitFences, vk::True, std::numeric_limits<uint64_t>::max());
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
        auto [acquireResult, imageIndex] = vkDevice.acquireNextImageKHR(mSwapchain->getSwapChain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore);
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

        const vklite::CommandBuffer &commandBuffer = (*mCommandBuffers)[mCurrentFrameIndex];
        commandBuffer.execute([&](const vk::CommandBuffer &vkCommandBuffer) {
            mRenderPass->execute(vkCommandBuffer, mFrameBuffers[imageIndex].getFrameBuffer(), [&](const vk::CommandBuffer &vkCommandBuffer) {
                vkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mGraphicsPipeline->getPipeline());
                vkCommandBuffer.setViewport(0, mViewports);
                vkCommandBuffer.setScissor(0, mScissors);
                std::array<vk::DeviceSize, 1> offsets = {0};
                std::array<vk::Buffer, 1> vertexBuffers = {mShaderStorageBuffers[mCurrentFrameIndex].getBuffer()};
                vkCommandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);
                vkCommandBuffer.draw(mParticleCount, 1, 0, 0);
            });
        });

        result = mSyncObject->resetFence(mCurrentFrameIndex);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("resetFences failed");
        }

        std::array<vk::Semaphore, 1> waitSemaphores = {imageAvailableSemaphore};
        std::array<vk::PipelineStageFlags, 1> waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        std::array<vk::CommandBuffer, 1> commandBuffers = {commandBuffer.getCommandBuffer()};
        std::array<vk::Semaphore, 1> signalSemaphores = {renderFinishedSemaphore};

        vk::SubmitInfo submitInfo{};
        submitInfo
                .setWaitSemaphores(waitSemaphores)
                .setWaitDstStageMask(waitStages)
                .setCommandBuffers(commandBuffers)
                .setSignalSemaphores(signalSemaphores);

        std::array<vk::SubmitInfo, 1> submitInfos = {submitInfo};
        // todo:
//        mDevice->getGraphicsQueue().submit(submitInfos, fence);

        std::array<vk::SwapchainKHR, 1> swapChains = {mSwapchain->getSwapChain()};
        std::array<uint32_t, 1> imageIndices = {imageIndex};
        vk::PresentInfoKHR presentInfo{};
        presentInfo
                .setWaitSemaphores(signalSemaphores)
                .setSwapchains(swapChains)
                .setImageIndices(imageIndices);

        // https://github.com/KhronosGroup/Vulkan-Hpp/issues/599
        // 当出现图片不匹配时， cpp风格的 presentKHR 会抛出异常， 而不是返回 result， 而C风格的 presentKHR 接口会返回 result
        try {
//            result = mDevice->getPresentQueue().presentKHR(presentInfo);
        } catch (const vk::OutOfDateKHRError &e) {
            LOG_E("mPresentQueue.presentKHR => OutOfDateKHRError");
            result = vk::Result::eErrorOutOfDateKHR;
        }

        if (result != vk::Result::eSuccess) {
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mFrameBufferResized) {
                mFrameBufferResized = false;
                LOG_E("presentKHR: eErrorOutOfDateKHR or eSuboptimalKHR or mFrameBufferResized, recreateSwapChain");
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
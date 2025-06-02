//
// Created by leixing on 2025/1/4.
//

#include "Test01SimpleTriangle.h"
#include "util/FileUtil.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/physical_device/msaa/MaxMsaaSampleCountSelector.h"
#include "vklite/vklite_android.h"

namespace test01 {

    Test01SimpleTriangle::Test01SimpleTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        LOG_D("Test01SimpleTriangle::Test01SimpleTriangle");

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

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/01_triangle.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/01_triangle.frag.spv");

        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

        mInstance = vklite::InstanceBuilder()
                .extensions({}, std::move(instanceExtensions))
                .layers({}, std::move(instanceLayers))
                .buildUnique();
        mSurface = vklite::AndroidSurfaceBuilder()
                .instance(mInstance->getInstance())
                .nativeWindow(mApp.window)
                .buildUnique();
        mPhysicalDevice = vklite::PhysicalDeviceSelector::makeDefault(*mSurface).selectUnique(mInstance->enumeratePhysicalDevices());

        vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
        if (mMsaaEnable) {
            sampleCount = vklite::MaxMsaaSampleCountSelector(4).select(mPhysicalDevice->querySampleCountFlagBits());
        }
        LOG_D("sampleCount:%d", sampleCount);

        std::vector<uint32_t> presentQueueFamilyIndices = mPhysicalDevice->queryQueueFamilyIndicesBySurface(mSurface->getSurface());
        std::vector<uint32_t> graphicQueueFamilyIndices = mPhysicalDevice->queryQueueFamilyIndicesByFlags(vk::QueueFlagBits::eGraphics);

        mDevice = vklite::DeviceBuilder()
                .physicalDevice(mPhysicalDevice->getPhysicalDevice())
                .extensions(std::move(deviceExtensions))
                .layers(std::move(deviceLayers))
                .addQueueFamily(graphicQueueFamilyIndices[0])
                .addQueueFamily(presentQueueFamilyIndices[0])
//                .addDevicePlugin(std::make_unique<vklite::AndroidPlugin>())
                .buildUnique();

        mGraphicQueue = std::make_unique<vklite::Queue>(mDevice->getQueue(graphicQueueFamilyIndices[0], 0));
        mPresentQueue = std::make_unique<vklite::Queue>(mDevice->getQueue(presentQueueFamilyIndices[0], 0));

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
                .queueFamilyIndex(graphicQueueFamilyIndices[0])
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
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3);
                })
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniform([&](vklite::UniformConfigure &uniformConfigure) {
                                uniformConfigure
                                        .binding(0)
                                        .shaderStageFlags(vk::ShaderStageFlagBits::eVertex);
                            });
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
                .configShader(graphicShaderConfigure)
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .buildUnique();

        uint32_t indicesSize = indices.size() * sizeof(uint32_t);
        mIndexBuffer = vklite::IndexBufferBuilder()
                .device(mDevice->getDevice())
                .configDeviceMemory(mPhysicalDevice->getPhysicalDevice())
                .size(indicesSize)
                .buildUnique();
        mIndexBuffer->update(*mCommandPool, indices);

        uint32_t verticesSize = vertices.size() * sizeof(Vertex);
        mVertexBuffer = vklite::VertexBufferBuilder()
                .device(mDevice->getDevice())
                .configDeviceMemory(mPhysicalDevice->getPhysicalDevice())
                .size(verticesSize)
                .buildUnique();
        mVertexBuffer->update(*mCommandPool, vertices.data(), verticesSize);

        LOG_D("test created ");
    }

    void Test01SimpleTriangle::init() {

    }

    // 检查是否准备好
    bool Test01SimpleTriangle::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test01SimpleTriangle::drawFrame() {
        const vk::Device vkDevice = mDevice->getDevice();
        vklite::Semaphore &imageAvailableSemaphore = mImageAvailableSemaphores[mCurrentFrameIndex];
        vklite::Semaphore &renderFinishedSemaphore = mRenderFinishedSemaphores[mCurrentFrameIndex];
        vklite::Fence &fence = mFences[mCurrentFrameIndex];

        vk::Result result = mFences[mCurrentFrameIndex].wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
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

                std::array<vk::Buffer, 1> vertexBuffers = {mVertexBuffer->getBuffer()};
                vkCommandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);

                vkCommandBuffer.bindIndexBuffer(mIndexBuffer->getBuffer(), 0, vk::IndexType::eUint32);

                vkCommandBuffer.drawIndexed(3, 1, 0, 0, 0);
            });
        });

        result = mFences[mCurrentFrameIndex].reset();
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("resetFences failed");
        }

        mGraphicQueue->submit(commandBuffer.getCommandBuffer(),
                              vk::PipelineStageFlagBits::eColorAttachmentOutput,
                              imageAvailableSemaphore.getSemaphore(),
                              renderFinishedSemaphore.getSemaphore(),
                              fence.getFence());

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
    void Test01SimpleTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test
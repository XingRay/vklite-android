//
// Created by leixing on 2025/1/4.
//

#include "Test01SimpleTriangle.h"
#include "FileUtil.h"
#include "vklite/platform/android/surface/AndroidSurfaceBuilder.h"
#include "vklite/physical_device/msaa/MaxMsaaSampleCountSelector.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/platform/android/device/AndroidDevicePlugin.h"

namespace test01 {

    Test01SimpleTriangle::Test01SimpleTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        LOG_D("Test01SimpleTriangle::Test01SimpleTriangle");

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

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/01_triangle.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/01_triangle.frag.spv");

        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

        mInstance = vklite::InstanceBuilder()
                .extensions({}, std::move(instanceExtensions))
                .layers({}, std::move(instanceLayers))
                .build();
        mSurface = vklite::AndroidSurfaceBuilder(mApp.window).build(*mInstance);
        mPhysicalDevice = vklite::PhysicalDeviceSelector::makeDefault(*mSurface)->select(mInstance->listPhysicalDevices());

        vk::SampleCountFlagBits sampleCountFlagBits = mPhysicalDevice->selectMaxMsaaSampleCountFlagBits(4);
        vklite::QueueFamilyIndices queueFamilyIndices = mPhysicalDevice->queryQueueFamilies(mSurface->getSurface(), vk::QueueFlagBits::eGraphics);

        mDevice = vklite::DeviceBuilder()
                .extensions(std::move(deviceExtensions))
                .layers(std::move(deviceLayers))
                .addGraphicQueueIndex(queueFamilyIndices.graphicQueueFamilyIndex.value())
                .addPresentQueueIndex(queueFamilyIndices.presentQueueFamilyIndex.value())
                .addDevicePlugin(std::make_unique<vklite::AndroidDevicePlugin>())
                .build(*mPhysicalDevice);

        mSwapchain = vklite::SwapchainBuilder().build(*mDevice, *mSurface);
        mCommandPool = vklite::CommandPoolBuilder().build(*mDevice);
        mRenderPass = vklite::RenderPassBuilder().build(*mDevice, *mSwapchain);
        mFrameBuffer = vklite::FrameBufferBuilder()
                .displaySize(mSwapchain->getDisplaySize())
                .displayFormat(mSwapchain->getDisplayFormat())
                .sampleCountFlagBits(sampleCountFlagBits)
                .build(*mDevice, *mRenderPass, *mCommandPool, mSwapchain->getDisplayImageViews());

        mSyncObject = vklite::SyncObjectBuilder()
                .frameCount(mFrameCount)
                .build(*mDevice);

        mGraphicsPipeline = vklite::GraphicsPipelineBuilder()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3);
                })
                .build(*mDevice, *mSwapchain, *mRenderPass);

//        mVkLiteEngine = vklite::VkLiteEngineBuilder{}
//                .layers({}, std::move(layers))
//                .extensions({}, std::move(instanceExtensions))
//                .deviceExtensions(std::move(deviceExtensions))
//                .surfaceBuilder(std::make_unique<vklite::AndroidVulkanSurfaceBuilder>(mApp.window))
//                .enableMsaa()
//                .physicalDeviceAsDefault()
//                .graphicsPipeline([&](vklite::GraphicsPipelineConfigure &graphicsPipelineConfigure) {
//                    graphicsPipelineConfigure
//                            .vertexShaderCode(std::move(vertexShaderCode))
//                            .fragmentShaderCode(std::move(fragmentShaderCode))
//                            .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
//                                vertexBindingConfigure
//                                        .binding(0)
//                                        .stride(sizeof(Vertex))
//                                        .addAttribute(0, ShaderFormat::Vec3);
//                            });
//                })
////                .pipelineResource([&](){
////
////                })
//                .build();



    }

    void Test01SimpleTriangle::init() {
//        mVkLiteEngine->getGraphicsPipeline()
//                .createVertexBuffer(vertices.size() * sizeof(Vertex))
//                .updateVertexBuffer(mVkLiteEngine->getCommandPool(), vertices)
//                .createIndexBuffer(indices.size() * sizeof(uint32_t))
//                .updateIndexBuffer(mVkLiteEngine->getCommandPool(), indices);

//        (*mVkLiteEngine)
//                .createVertexBuffer(vertices.size() * sizeof(Vertex))
//                .updateVertexBuffer(vertices)
//                .createIndexBuffer(indices.size() * sizeof(uint32_t))
//                .updateIndexBuffer(indices);

    }

    // 检查是否准备好
    bool Test01SimpleTriangle::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test01SimpleTriangle::drawFrame() {
        const vk::Device device = mDevice->getDevice();
        vk::Semaphore imageAvailableSemaphore = mSyncObject->getImageAvailableSemaphore(mCurrentFrameIndex);
        vk::Semaphore renderFinishedSemaphore = mSyncObject->getRenderFinishedSemaphore(mCurrentFrameIndex);
        vk::Fence fence = mSyncObject->getFence(mCurrentFrameIndex);
        std::array<vk::Fence, 1> waitFences = {fence};

        vk::Result result = device.waitForFences(waitFences, vk::True, std::numeric_limits<uint64_t>::max());
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
        auto [acquireResult, imageIndex] = device.acquireNextImageKHR(mSwapchain->getSwapChain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore);
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

        vk::CommandBufferBeginInfo commandBufferBeginInfo;
        commandBufferBeginInfo
//                .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
                .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse)
                .setPInheritanceInfo(nullptr);

        vk::CommandBuffer commandBuffer = mCommandPool->getCommandBuffers()[mCurrentFrameIndex];
        commandBuffer.reset();
        commandBuffer.begin(commandBufferBeginInfo);

        vk::Extent2D displaySize = mSwapchain->getDisplaySize();
        vk::Rect2D renderArea{};
        renderArea
                .setOffset(vk::Offset2D{0, 0})
                .setExtent(displaySize);

        vk::ClearValue colorClearValue = vk::ClearValue{mClearColor};
        vk::ClearValue depthStencilClearValue = vk::ClearValue{vk::ClearColorValue(mDepthStencil)};
        std::array<vk::ClearValue, 2> clearValues = {colorClearValue, depthStencilClearValue};

        vk::RenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo
                .setRenderPass(mRenderPass->getRenderPass())
                .setFramebuffer(mFrameBuffer->getFrameBuffers()[imageIndex])
                .setRenderArea(renderArea)
                .setClearValues(clearValues);
        /**
         * vk::SubpassContents::eInline
         * 子流程的渲染命令直接记录在当前的命令缓冲区中, 适用于简单的渲染流程，所有渲染命令都在同一个命令缓冲区中记录。
         *
         * vk::SubpassContents::eSecondaryCommandBuffers
         * 子流程的渲染命令通过次级命令缓冲区（Secondary Command Buffer）记录。
         * 适用于复杂的渲染流程，可以将渲染命令分散到多个次级命令缓冲区中，以提高代码的模块化和复用性。
         */
        commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        mGraphicsPipeline->drawFrame(commandBuffer, mPipelineResources[mCurrentFrameIndex]);

        commandBuffer.endRenderPass();
        commandBuffer.end();

        result = mSyncObject->resetFence(mCurrentFrameIndex);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("resetFences failed");
        }

        std::array<vk::Semaphore, 1> waitSemaphores = {imageAvailableSemaphore};
        std::array<vk::PipelineStageFlags, 1> waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        std::array<vk::CommandBuffer, 1> commandBuffers = {commandBuffer};
        std::array<vk::Semaphore, 1> signalSemaphores = {renderFinishedSemaphore};

        vk::SubmitInfo submitInfo{};
        submitInfo
                .setWaitSemaphores(waitSemaphores)
                .setWaitDstStageMask(waitStages)
                .setCommandBuffers(commandBuffers)
                .setSignalSemaphores(signalSemaphores);

        std::array<vk::SubmitInfo, 1> submitInfos = {submitInfo};
        mDevice->getGraphicsQueue().submit(submitInfos, fence);

        std::array<vk::SwapchainKHR, 1> swapChains = {mSwapchain->getSwapChain()};
        std::array<uint32_t, 1> imageIndices = {imageIndex};
        vk::PresentInfoKHR presentInfo{};
        presentInfo
                .setWaitSemaphores(signalSemaphores)
                .setSwapchains(swapChains)
                .setImageIndices(imageIndices);

//    std::cout << "presentKHR, mFrameBufferResized:" << mFrameBufferResized << std::endl;

        // https://github.com/KhronosGroup/Vulkan-Hpp/issues/599
        // 当出现图片不匹配时， cpp风格的 presentKHR 会抛出异常， 而不是返回 result， 而C风格的 presentKHR 接口会返回 result
        try {
            result = mDevice->getPresentQueue().presentKHR(presentInfo);
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
    void Test01SimpleTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test
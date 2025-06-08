//
// Created by leixing on 2025/1/4.
//

#include "Test07NdkCamera.h"

#include <chrono>
#include <thread>

#include "util/FileUtil.h"

namespace std {

    template<>
    struct hash<test07::Vertex> {
        size_t operator()(test07::Vertex const &vertex) const {
            size_t seed = 0;

            // 哈希 position
            hash<glm::vec3> vec3Hash;
            seed ^= vec3Hash(vertex.position) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            // 哈希 uv
            hash<glm::vec2> vec2Hash;
            seed ^= vec2Hash(vertex.uv) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}


namespace test07 {
    Test07NdkCamera::Test07NdkCamera(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        mNdkCamera = std::make_unique<ndkcamera::NdkCamera>();
        mNdkCamera->startPreview();

        AHardwareBuffer *hardwareBuffer = nullptr;
        while (hardwareBuffer == nullptr) {
            LOG_D("waiting for getLatestHardwareBuffer...");
            std::optional<ndkcamera::Image> image = mNdkCamera->acquireLatestImage();
            if (!image.has_value()) {
                continue;
            }
            hardwareBuffer = image.value().getHardwareBuffer();
        }

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/07_ndk_camera.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/07_ndk_camera.frag.spv");

        vklite::ShaderConfigure shaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3)
                            .addAttribute(1, ShaderFormat::Vec2);
                })
                .addPushConstant(0, sizeof(glm::mat4), vk::ShaderStageFlagBits::eVertex)
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addImmutableSampler(0, {}, vk::ShaderStageFlagBits::eFragment);
                });

        mEngine = vklite::AndroidGraphicPipelineEngineBuilder::asDefault(mApp.window)
                .addInstancePlugin(vklite::HardwareBufferPlugin::buildUnique())
                .addDevicePlugin(vklite::HardwareBufferPlugin::buildUnique())
                .shaderConfigure(std::move(shaderConfigure))
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();

//        mHardwareBufferImage = std::make_unique<vklite::HardwareBufferImage>(*mPhysicalDevice, *mDevice, vkHardwareBuffer, *mConversion);
//        mHardwareBufferImageView = vklite::HardwareBufferImageViewBuilder()
//                .format(vkHardwareBuffer.getFormatProperties().format)
//                .conversion((*mConversion).getSamplerYcbcrConversion())
//                .buildUnique(*mDevice, *mHardwareBufferImage);
//
//        vklite::DescriptorSetWriter descriptorSetWriter = vklite::DescriptorSetWriterBuilder()
//                .frameCount(mFrameCount)
//                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &descriptorSetMappingConfigure) {
//                    descriptorSetMappingConfigure
//                            .addMapping([&](vklite::DescriptorMapping &mapping) {
//                                mapping
//                                        .binding(0)
//                                        .descriptorSet(mPipelineResources[frameIndex].getDescriptorSets()[0])
//                                        .descriptorType(vk::DescriptorType::eCombinedImageSampler)
//                                        .addImageInfo(*mHardwareBufferSampler, *mHardwareBufferImageView);
//                            });
//                })
//                .build();
//        mDevice->getDevice().updateDescriptorSets(descriptorSetWriter.createWriteDescriptorSets(), nullptr);

        mFrameCounter = std::make_unique<util::FrameCounter>();
        LOG_D("test created ");
    }

    void Test07NdkCamera::init() {
// x轴朝右, y轴朝下, z轴朝前, 右手系 (x,y)->z
        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // 左上角
                {{1.0f,  -1.0f, 0.0f}, {0.0f, 0.0f}}, // 右上角
                {{-1.0f, 1.0f,  0.0f}, {1.0f, 1.0f}}, // 左下角
                {{1.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // 右下角
        };

        std::vector<uint32_t> indices = {0, 2, 1, 1, 2, 3};
    }

    // 检查是否准备好
    bool Test07NdkCamera::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test07NdkCamera::drawFrame() {
//        LOG_D("Test07NdkCamera::drawFrame()");
        std::optional<ndkcamera::Image> image = mNdkCamera->acquireLatestImage();
        if (!image.has_value()) {
            LOG_D("Test07NdkCamera::drawFrame(), no image");
            return;
        }
        AHardwareBuffer *hardwareBuffer = image.value().getHardwareBuffer();
        if (hardwareBuffer == nullptr) {
            LOG_D("Test07NdkCamera::drawFrame(), no hardwareBuffer");
            return;
        }

//        vklite::HardwareBuffer vkHardwareBuffer = vklite::HardwareBuffer::build(*mDevice, hardwareBuffer);
//        mHardwareBufferImage = std::make_unique<vklite::HardwareBufferImage>(*mPhysicalDevice, *mDevice, vkHardwareBuffer, *mConversion);
//        mHardwareBufferImageView = vklite::HardwareBufferImageViewBuilder()
//                .format(vkHardwareBuffer.getFormatProperties().format)
//                .conversion((*mConversion).getSamplerYcbcrConversion())
//                .buildUnique(*mDevice, *mHardwareBufferImage);
//
//        vklite::DescriptorSetWriter descriptorSetWriter = vklite::DescriptorSetWriterBuilder()
//                .frameCount(mFrameCount)
//                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &descriptorSetMappingConfigure) {
//                    descriptorSetMappingConfigure
//                            .addMapping([&](vklite::DescriptorMapping &mapping) {
//                                mapping
//                                        .binding(0)
//                                        .descriptorSet(mPipelineResources[frameIndex].getDescriptorSets()[0])
//                                        .descriptorType(vk::DescriptorType::eCombinedImageSampler)
//                                        .addImageInfo(*mHardwareBufferSampler, *mHardwareBufferImageView);
//                            });
//                })
//                .build();
//        mDevice->getDevice().updateDescriptorSets(descriptorSetWriter.createWriteDescriptorSets(), nullptr);
//
//        const vk::Device vkDevice = mDevice->getDevice();
//        vk::Semaphore imageAvailableSemaphore = mSyncObject->getImageAvailableSemaphore(mCurrentFrameIndex);
//        vk::Semaphore renderFinishedSemaphore = mSyncObject->getRenderFinishedSemaphore(mCurrentFrameIndex);
//        vk::Fence fence = mSyncObject->getFence(mCurrentFrameIndex);
//        std::array<vk::Fence, 1> waitFences = {fence};
//
//        vk::Result result = vkDevice.waitForFences(waitFences, vk::True, std::numeric_limits<uint64_t>::max());
//        if (result != vk::Result::eSuccess) {
//            LOG_E("waitForFences failed");
//            throw std::runtime_error("waitForFences failed");
//        }
//
//        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
//        auto [acquireResult, imageIndex] = vkDevice.acquireNextImageKHR(mSwapchain->getSwapChain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore);
//        if (acquireResult != vk::Result::eSuccess) {
//            if (acquireResult == vk::Result::eErrorOutOfDateKHR) {
//                // 交换链已与表面不兼容，不能再用于渲染。通常在窗口大小调整后发生。
//                LOG_E("acquireNextImageKHR: eErrorOutOfDateKHR, recreateSwapChain");
////                recreateSwapChain();
//                return;
//            } else if (acquireResult == vk::Result::eSuboptimalKHR) {
//                //vk::Result::eSuboptimalKHR 交换链仍然可以成功显示到表面，但表面属性不再完全匹配。
//                LOG_D("acquireNextImageKHR: eSuboptimalKHR");
//            } else {
//                LOG_E("acquireNextImageKHR: failed: %d", acquireResult);
//                throw std::runtime_error("acquireNextImageKHR failed");
//            }
//        }
//
//        const vklite::PooledCommandBuffer &commandBuffer = (*mCommandBuffers)[mCurrentFrameIndex];
//        commandBuffer.record([&](const vk::CommandBuffer &vkCommandBuffer) {
//            mRenderPass->execute(vkCommandBuffer, mFramebuffers[imageIndex].getFrameBuffer(), [&](const vk::CommandBuffer &vkCommandBuffer) {
//                mGraphicsPipeline->drawFrame(vkCommandBuffer, *mPipelineLayout, mPipelineResources[mCurrentFrameIndex], mViewports, mScissors);
//            });
//        });
//
//        result = mSyncObject->resetFence(mCurrentFrameIndex);
//        if (result != vk::Result::eSuccess) {
//            throw std::runtime_error("resetFences failed");
//        }
//
//        std::array<vk::Semaphore, 1> waitSemaphores = {imageAvailableSemaphore};
//        std::array<vk::PipelineStageFlags, 1> waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
//        std::array<vk::CommandBuffer, 1> commandBuffers = {commandBuffer.getCommandBuffer()};
//        std::array<vk::Semaphore, 1> signalSemaphores = {renderFinishedSemaphore};
//
//        vk::SubmitInfo submitInfo{};
//        submitInfo
//                .setWaitSemaphores(waitSemaphores)
//                .setWaitDstStageMask(waitStages)
//                .setCommandBuffers(commandBuffers)
//                .setSignalSemaphores(signalSemaphores);
//
//        std::array<vk::SubmitInfo, 1> submitInfos = {submitInfo};
////        mDevice->getGraphicsQueue().submit(submitInfos, fence);
//
//        std::array<vk::SwapchainKHR, 1> swapChains = {mSwapchain->getSwapChain()};
//        std::array<uint32_t, 1> imageIndices = {imageIndex};
//        vk::PresentInfoKHR presentInfo{};
//        presentInfo
//                .setWaitSemaphores(signalSemaphores)
//                .setSwapchains(swapChains)
//                .setImageIndices(imageIndices);
//
//        // https://github.com/KhronosGroup/Vulkan-Hpp/issues/599
//        // 当出现图片不匹配时， cpp风格的 presentKHR 会抛出异常， 而不是返回 result， 而C风格的 presentKHR 接口会返回 result
//        try {
////            result = mDevice->getPresentQueue().presentKHR(presentInfo);
//        } catch (const vk::OutOfDateKHRError &e) {
//            LOG_E("mPresentQueue.presentKHR => OutOfDateKHRError");
//            result = vk::Result::eErrorOutOfDateKHR;
//        }
//
//        if (result != vk::Result::eSuccess) {
//            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mFramebufferResized) {
//                mFramebufferResized = false;
//                LOG_E("presentKHR: eErrorOutOfDateKHR or eSuboptimalKHR or mFramebufferResized, recreateSwapChain");
//                // todo: recreateSwapChain
////                recreateSwapChain();
//                return;
//            } else {
//                throw std::runtime_error("presentKHR failed");
//            }
//        }
//
//        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mFrameCount;
//
//
//        // 增加帧计数器
//        mFrameCounter->count();
//        LOG_D("FPS: %.2f", mFrameCounter->getFps());
    }

    // 清理操作
    void Test07NdkCamera::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test

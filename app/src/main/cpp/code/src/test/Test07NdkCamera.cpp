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

        std::vector<const char*> instanceExtensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,

                // old version
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                // new version
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,

                // hardware buffer
                VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME,
                VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
        };

        std::vector<const char*> instanceLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,

                // hardware buffer
                VK_KHR_MAINTENANCE1_EXTENSION_NAME,
                VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
                VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
                VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME,
                VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME,
                VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME,
                VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
                VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
        };

        std::vector<const char *> deviceLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/07_ndk_camera.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/07_ndk_camera.frag.spv");

        // x轴朝右, y轴朝下, z轴朝前, 右手系 (x,y)->z
        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // 左上角
                {{1.0f,  -1.0f, 0.0f}, {0.0f, 0.0f}}, // 右上角
                {{-1.0f, 1.0f,  0.0f}, {1.0f, 1.0f}}, // 左下角
                {{1.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // 右下角
        };

        std::vector<uint32_t> indices = {0, 2, 1, 1, 2, 3};

        mInstance = vklite::InstanceBuilder()
                .extensions({}, std::move(instanceExtensions))
                .layers({}, std::move(instanceLayers))
                .buildUnique();

        vklite::AndroidInstancePlugin().onInstanceCreated(*mInstance);

        mSurface = vklite::AndroidSurfaceBuilder(mApp.window).build(*mInstance);
        mPhysicalDevice = vklite::PhysicalDeviceSelector::makeDefault(*mSurface)->select(mInstance->enumeratePhysicalDevices());

        vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
        if (mMsaaEnable) {
            sampleCount = mPhysicalDevice->selectMaxMsaaSampleCountFlagBits(4);
        }
        LOG_D("sampleCount:%d", sampleCount);
        vklite::QueueFamilyIndices queueFamilyIndices = mPhysicalDevice->queryQueueFamilies(mSurface->getSurface(), vk::QueueFlagBits::eGraphics);

        mDevice = vklite::DeviceBuilder()
                .extensions(std::move(deviceExtensions))
                .layers(std::move(deviceLayers))
//                .addGraphicQueueIndex(queueFamilyIndices.graphicQueueFamilyIndex.value())
//                .addPresentQueueIndex(queueFamilyIndices.presentQueueFamilyIndex.value())
//                .addDevicePlugin(std::make_unique<vklite::AndroidPlugin>())
//                .buildUnique(*mPhysicalDevice);
                .buildUnique();

        mSwapchain = vklite::SwapchainBuilder()
                .build(*mPhysicalDevice, *mDevice, *mSurface, {/*todo*/});

        mCommandPool = vklite::CommandPoolBuilder()
//                .queueFamilyIndex(mDevice->getGraphicQueueFamilyIndex())
                .build(*mDevice);
        mCommandBuffers = mCommandPool->allocateUnique(mFrameCount);

        // 创建附件
        mDisplayImageViews = vklite::ImageViewBuilder::colorImageViewBuilder()
                .format(mSwapchain->getDisplayFormat())
                .build(*mDevice, mSwapchain->getDisplayImages());

        if (mMsaaEnable) {
            mColorImage = vklite::ImageBuilder::colorImageBuilder()
                    .size(mSwapchain->getDisplaySize())
                    .format(mSwapchain->getDisplayFormat())
                    .sampleCount(sampleCount)
                    .buildUnique(*mPhysicalDevice, *mDevice);
            mColorImageView = vklite::ImageViewBuilder::colorImageViewBuilder()
                    .format(mSwapchain->getDisplayFormat())
                    .buildUnique(*mDevice, *mColorImage);
        }

        if (mDepthTestEnable) {
            vk::Format depthFormat = mPhysicalDevice->findDepthFormat();

            mDepthImage = vklite::ImageBuilder::depthImageBuilder()
                    .size(mSwapchain->getDisplaySize())
                    .format(depthFormat)
                    .sampleCount(sampleCount)
                    .postCreated([&](vklite::Image &image) {
                        image.transitionImageLayout(*mCommandPool, vklite::ImageTransition::depthImageTransition());
                    })
                    .buildUnique(*mPhysicalDevice, *mDevice);

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

        vklite::HardwareBuffer vkHardwareBuffer = vklite::HardwareBuffer::build(*mDevice, hardwareBuffer);
        mConversion = std::make_unique<vklite::HardwareBufferYcbcrConversion>(*mDevice, vkHardwareBuffer.getFormatProperties());
        mHardwareBufferSampler = std::make_unique<vklite::HardwareBufferSampler>(*mDevice, *mConversion);

        vklite::DescriptorConfigure descriptorConfigure = vklite::DescriptorConfigure()
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addImmutableSampler([&](vklite::ImmutableSamplerConfigure &immutableSamplerConfigure) {
                                immutableSamplerConfigure
                                        .binding(0)
                                        .shaderStageFlags(vk::ShaderStageFlagBits::eFragment)
                                        .addSampler(*mHardwareBufferSampler);
                            });
                });

        mDescriptorPool = vklite::DescriptorPoolBuilder()
                .descriptorPoolSizes(descriptorConfigure.calcDescriptorPoolSizes())
                .descriptorSetCount(descriptorConfigure.getDescriptorSetCount())
                .frameCount(mFrameCount)
                .buildUnique(*mDevice);

        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = descriptorConfigure.createDescriptorSetLayouts(*mDevice);

        mPipelineLayout = vklite::PipelineLayoutBuilder()
//                .addPushConstant(sizeof(glm::mat4), 0, vk::ShaderStageFlagBits::eVertex)
                .buildUnique(*mDevice, descriptorSetLayouts);

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
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3)
                            .addAttribute(1, ShaderFormat::Vec2);
                })
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .buildUnique(*mDevice, *mRenderPass, *mPipelineLayout, mViewports, mScissors);

        mIndexBuffer = vklite::IndexBufferBuilder()
                .bufferSize(indices.size() * sizeof(uint32_t))
                .build(*mPhysicalDevice, *mDevice);
        mIndexBuffer->update(*mCommandPool, indices);

        mVertexBuffer = vklite::VertexBufferBuilder()
                .bufferSize(vertices.size() * sizeof(Vertex))
                .build(*mPhysicalDevice, *mDevice);
        mVertexBuffer->update(*mCommandPool, vertices);

        mPipelineResources = vklite::PipelineResourcesBuilder()
                .frameCount(mFrameCount)
                .pipelineResourceBuilder([&](uint32_t frameIndex) {
                    return vklite::PipelineResourceBuilder()
                            .addVertexBuffer(*mVertexBuffer)
                            .indexBuffer(*mIndexBuffer)
                            .indicesCount(indices.size())
                            .descriptorSets(mDescriptorPool->allocateDescriptorSets(descriptorSetLayouts))
                            .build();
                })
                .build();

        mHardwareBufferImage = std::make_unique<vklite::HardwareBufferImage>(*mPhysicalDevice, *mDevice, vkHardwareBuffer, *mConversion);
        mHardwareBufferImageView = vklite::HardwareBufferImageViewBuilder()
                .format(vkHardwareBuffer.getFormatProperties().format)
                .conversion((*mConversion).getSamplerYcbcrConversion())
                .buildUnique(*mDevice, *mHardwareBufferImage);

        vklite::DescriptorSetWriter descriptorSetWriter = vklite::DescriptorSetWriterBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &descriptorSetMappingConfigure) {
                    descriptorSetMappingConfigure
                            .addMapping([&](vklite::DescriptorMapping &mapping) {
                                mapping
                                        .binding(0)
                                        .descriptorSet(mPipelineResources[frameIndex].getDescriptorSets()[0])
                                        .descriptorType(vk::DescriptorType::eCombinedImageSampler)
                                        .addImageInfo(*mHardwareBufferSampler, *mHardwareBufferImageView);
                            });
                })
                .build();
        mDevice->getDevice().updateDescriptorSets(descriptorSetWriter.createWriteDescriptorSets(), nullptr);

        mFrameCounter = std::make_unique<util::FrameCounter>();
        LOG_D("test created ");
    }

    void Test07NdkCamera::init() {

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

        vklite::HardwareBuffer vkHardwareBuffer = vklite::HardwareBuffer::build(*mDevice, hardwareBuffer);
        mHardwareBufferImage = std::make_unique<vklite::HardwareBufferImage>(*mPhysicalDevice, *mDevice, vkHardwareBuffer, *mConversion);
        mHardwareBufferImageView = vklite::HardwareBufferImageViewBuilder()
                .format(vkHardwareBuffer.getFormatProperties().format)
                .conversion((*mConversion).getSamplerYcbcrConversion())
                .buildUnique(*mDevice, *mHardwareBufferImage);

        vklite::DescriptorSetWriter descriptorSetWriter = vklite::DescriptorSetWriterBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &descriptorSetMappingConfigure) {
                    descriptorSetMappingConfigure
                            .addMapping([&](vklite::DescriptorMapping &mapping) {
                                mapping
                                        .binding(0)
                                        .descriptorSet(mPipelineResources[frameIndex].getDescriptorSets()[0])
                                        .descriptorType(vk::DescriptorType::eCombinedImageSampler)
                                        .addImageInfo(*mHardwareBufferSampler, *mHardwareBufferImageView);
                            });
                })
                .build();
        mDevice->getDevice().updateDescriptorSets(descriptorSetWriter.createWriteDescriptorSets(), nullptr);

        const vk::Device vkDevice = mDevice->getDevice();
        vk::Semaphore imageAvailableSemaphore = mSyncObject->getImageAvailableSemaphore(mCurrentFrameIndex);
        vk::Semaphore renderFinishedSemaphore = mSyncObject->getRenderFinishedSemaphore(mCurrentFrameIndex);
        vk::Fence fence = mSyncObject->getFence(mCurrentFrameIndex);
        std::array<vk::Fence, 1> waitFences = {fence};

        vk::Result result = vkDevice.waitForFences(waitFences, vk::True, std::numeric_limits<uint64_t>::max());
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
        commandBuffer.record([&](const vk::CommandBuffer &vkCommandBuffer) {
            mRenderPass->execute(vkCommandBuffer, mFrameBuffers[imageIndex].getFrameBuffer(), [&](const vk::CommandBuffer &vkCommandBuffer) {
                mGraphicsPipeline->drawFrame(vkCommandBuffer, *mPipelineLayout, mPipelineResources[mCurrentFrameIndex], mViewports, mScissors);
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


        // 增加帧计数器
        mFrameCounter->count();
        LOG_D("FPS: %.2f", mFrameCounter->getFps());
    }

    // 清理操作
    void Test07NdkCamera::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test

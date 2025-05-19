//
// Created by leixing on 2025/1/4.
//

#include "Test05TextureImage.h"
#include "util/FileUtil.h"

#include "util/FileUtil.h"
#include "vklite/vklite_android.h"
#include "stb_image.h"
#include "image/StbImage.h"

namespace test05 {

    Test05TextureImage::Test05TextureImage(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app), mMvpMatrix(glm::mat4(1.0f)) {

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

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/05_texture_image.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/05_texture_image.frag.spv");

        std::vector<Vertex> vertices = {
                // 前表面 (Z轴正方向)
                {{-1.0f, -1.0f, 1.0f},  {0.0f, 0.0f}}, // 左下
                {{1.0f,  -1.0f, 1.0f},  {1.0f, 0.0f}},  // 右下
                {{1.0f,  1.0f,  1.0f},  {1.0f, 1.0f}},  // 右上
                {{-1.0f, 1.0f,  1.0f},  {0.0f, 1.0f}},  // 左上

                // 后表面 (Z轴负方向)
                {{1.0f,  -1.0f, -1.0f}, {0.0f, 0.0f}},  // 右下
                {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},  // 左下
                {{-1.0f, 1.0f,  -1.0f}, {1.0f, 1.0f}},  // 左上
                {{1.0f,  1.0f,  -1.0f}, {0.0f, 1.0f}},  // 右上

                // 左表面 (X轴负方向)
                {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},  // 后下
                {{-1.0f, -1.0f, 1.0f},  {1.0f, 0.0f}},  // 前下
                {{-1.0f, 1.0f,  1.0f},  {1.0f, 1.0f}},  // 前上
                {{-1.0f, 1.0f,  -1.0f}, {0.0f, 1.0f}},  // 后上

                // 右表面 (X轴正方向)
                {{1.0f,  -1.0f, 1.0f},  {0.0f, 0.0f}},  // 前下
                {{1.0f,  -1.0f, -1.0f}, {1.0f, 0.0f}},  // 后下
                {{1.0f,  1.0f,  -1.0f}, {1.0f, 1.0f}},  // 后上
                {{1.0f,  1.0f,  1.0f},  {0.0f, 1.0f}},  // 前上

                // 上表面 (Y轴正方向)
                {{-1.0f, 1.0f,  1.0f},  {0.0f, 0.0f}},  // 前左
                {{1.0f,  1.0f,  1.0f},  {1.0f, 0.0f}},  // 前右
                {{1.0f,  1.0f,  -1.0f}, {1.0f, 1.0f}},  // 后右
                {{-1.0f, 1.0f,  -1.0f}, {0.0f, 1.0f}},  // 后左

                // 下表面 (Y轴负方向)
                {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},  // 后左
                {{1.0f,  -1.0f, -1.0f}, {1.0f, 0.0f}},  // 后右
                {{1.0f,  -1.0f, 1.0f},  {1.0f, 1.0f}},  // 前右
                {{-1.0f, -1.0f, 1.0f},  {0.0f, 1.0f}}   // 前左
        };

        std::vector<uint32_t> indices = {
                // 前表面
                0, 1, 2, 2, 3, 0,
                // 后表面
                4, 5, 6, 6, 7, 4,
                // 左表面
                8, 9, 10, 10, 11, 8,
                // 右表面
                12, 13, 14, 14, 15, 12,
                // 上表面
                16, 17, 18, 18, 19, 16,
                // 下表面
                20, 21, 22, 22, 23, 20
        };


        mMvpMatrix = MvpMatrix{};
        float scale = 1.0f;

        float screenWidth = ANativeWindow_getWidth(mApp.window);
        float screenHeight = ANativeWindow_getHeight(mApp.window);
        float aspectRatio = screenWidth / screenHeight;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;
        mMvpMatrix.view = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f),
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.proj = glm::perspective(glm::radians(45.0f),
                                           aspectRatio,
                                           1.0f,
                                           20.0f);

        std::unique_ptr<image::ImageInterface> textureImage = image::StbImage::loadImage("/storage/emulated/0/01.png");

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
        vklite::QueueFamilyIndices queueFamilyIndices = mPhysicalDevice->queryQueueFamilies(mSurface->getSurface(), vk::QueueFlagBits::eGraphics);

        mDevice = vklite::DeviceBuilder()
                .extensions(std::move(deviceExtensions))
                .layers(std::move(deviceLayers))
                .addGraphicQueueIndex(queueFamilyIndices.graphicQueueFamilyIndex.value())
                .addPresentQueueIndex(queueFamilyIndices.presentQueueFamilyIndex.value())
                .addDevicePlugin(std::make_unique<vklite::AndroidPlugin>())
                .build(*mPhysicalDevice);

        mSwapchain = vklite::SwapchainBuilder()
                .build(*mDevice, *mSurface);

        mCommandPool = vklite::CommandPoolBuilder()
                .queueFamilyIndex(mDevice->getGraphicQueueFamilyIndex())
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
                    .buildUnique(*mDevice);
            mColorImageView = vklite::ImageViewBuilder::colorImageViewBuilder()
                    .format(mSwapchain->getDisplayFormat())
                    .build(*mDevice, *mColorImage);
        }

        if (mDepthTestEnable) {
            vk::Format depthFormat = mPhysicalDevice->findDepthFormat();

            std::unique_ptr<vklite::Image> depthImage = vklite::ImageBuilder::depthImageBuilder()
                    .width(mSwapchain->getDisplaySize().width)
                    .height(mSwapchain->getDisplaySize().height)
                    .format(depthFormat)
                    .sampleCount(sampleCount)
                    .buildUnique(*mDevice);
            depthImage->transitionImageLayout(*mCommandPool, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1,
                                              vk::QueueFamilyIgnored, vk::QueueFamilyIgnored, vk::ImageAspectFlagBits::eDepth);
            mDepthImage = std::move(depthImage);

            mDepthImageView = vklite::ImageViewBuilder::depthImageViewBuilder()
                    .format(depthFormat)
                    .build(*mDevice, *mDepthImage);
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

        mFrameBuffers.reserve(mDisplayImageViews.size());
        for (const vklite::ImageView &imageView: mDisplayImageViews) {
            mFrameBuffers.push_back(vklite::FrameBufferBuilder()
                                            .width(mSwapchain->getDisplaySize().width)
                                            .height(mSwapchain->getDisplaySize().height)
                                                    // 下面添加附件的顺序不能乱, 附件的顺序由 RenderPass 的附件定义顺序决定，必须严格一致。
                                            .addAttachmentIf(mMsaaEnable, [&]() { return mColorImageView->getImageView(); })
                                            .addAttachment(imageView.getImageView())
                                            .addAttachmentIf(mDepthTestEnable, [&]() { return mDepthImageView->getImageView(); })
                                            .build(*mDevice, *mRenderPass));
        }

        mSyncObject = vklite::SyncObjectBuilder()
                .frameCount(mFrameCount)
                .build(*mDevice);

        vklite::DescriptorConfigure descriptorConfigure = vklite::DescriptorConfigure()
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addSampler([](vklite::SamplerConfigure &samplerConfigure) {
                                samplerConfigure
                                        .binding(0)
                                        .shaderStageFlags(vk::ShaderStageFlagBits::eFragment);
                            });
                });

        mDescriptorPool = vklite::DescriptorPoolBuilder()
                .descriptorPoolSizes(descriptorConfigure.calcDescriptorPoolSizes())
                .descriptorSetCount(descriptorConfigure.getDescriptorSetCount())
                .frameCount(mFrameCount)
                .buildUnique(*mDevice);

        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = descriptorConfigure.createDescriptorSetLayouts(*mDevice);

        mPipelineLayout = vklite::PipelineLayoutBuilder()
                .addPushConstant(sizeof(glm::mat4), 0, vk::ShaderStageFlagBits::eVertex)
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
                .build(*mDevice);
        mIndexBuffer->update(*mCommandPool, indices);

        mVertexBuffer = vklite::VertexBufferBuilder()
                .bufferSize(vertices.size() * sizeof(Vertex))
                .build(*mDevice);
        mVertexBuffer->update(*mCommandPool, vertices);

        mPipelineResources = vklite::PipelineResourcesBuilder()
                .frameCount(mFrameCount)
                .pipelineResourceBuilder([&](uint32_t frameIndex) {
                    return vklite::PipelineResourceBuilder()
                            .addVertexBuffer(*mVertexBuffer)
                            .indexBuffer(*mIndexBuffer)
                            .indicesCount(indices.size())
                            .addPushConstant(vklite::PushConstant(&mMvpMatrix, sizeof(glm::mat4)))
                            .descriptorSets(mDescriptorPool->allocateDescriptorSets(descriptorSetLayouts))
                            .build();
                })
                .build();


        vklite::StagingBuffer stagingBuffer = vklite::StagingBuffer(*mDevice, textureImage->getPixelBytes());
        stagingBuffer.updateBuffer(textureImage->getPixels(), textureImage->getPixelBytes());

        for (int i = 0; i < mFrameCount; i++) {
            std::unique_ptr<vklite::Image> image = vklite::ImageBuilder::textureImageBuilder()
                    .width(textureImage->getWidth())
                    .height(textureImage->getHeight())
                    .format(textureImage->getFormat())
                    .buildUnique(*mDevice);
            mCommandPool->submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
                image->recordCommandTransitionImageLayout(commandBuffer, vk::ImageLayout::eUndefined, vk::ImageLayout::eUndefined, 1,
                                                          vk::QueueFamilyIgnored, vk::QueueFamilyIgnored, vk::ImageAspectFlagBits::eColor);
                image->recordCommandCopyDataFromBuffer(commandBuffer, stagingBuffer.getBuffer());
            });


            mImages.push_back(std::move(image));
            mImageViews.push_back(vklite::ImageViewBuilder()
                                          .format(textureImage->getFormat())
                                          .build(*mDevice, *mImages[i]));
            mSamplers.push_back(vklite::SamplerBuilder().build(*mDevice));
        }

        vklite::DescriptorSetWriter descriptorSetWriter = vklite::DescriptorSetWriterBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &descriptorSetMappingConfigure) {
                    descriptorSetMappingConfigure
                            .addMapping([&](vklite::DescriptorMapping &mapping) {
                                mapping
                                        .descriptorSet(mPipelineResources[frameIndex].getDescriptorSets()[0])
//                                        .binding(0)
                                        .descriptorType(vk::DescriptorType::eCombinedImageSampler)
//                                        .descriptorIndex(0)
//                                        .descriptorCount(1)
                                        .addImageInfo(*mSamplers[frameIndex], *mImageViews[frameIndex]);
                            });
                })
                .build();
        mDevice->getDevice().updateDescriptorSets(descriptorSetWriter.createWriteDescriptorSets(), nullptr);

        LOG_D("test created ");
    }

    void Test05TextureImage::init() {

    }

    // 检查是否准备好
    bool Test05TextureImage::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test05TextureImage::drawFrame() {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float progress = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        float scale = 1.0f;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, progress * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;

//        mMvpMatrix.model = glm::mat4(1.0f); // 单位矩阵
//        mMvpMatrix.view = glm::mat4(1.0f);  // 单位矩阵
//        mMvpMatrix.proj = glm::mat4(1.0f);  // 单位矩阵
        glm::mat4 mvp = mMvpMatrix.proj * mMvpMatrix.view * mMvpMatrix.model;
        mPipelineResources[mCurrentFrameIndex].updatePushConstant(0, &mvp, sizeof(glm::mat4));

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
        commandBuffer.execute([&](const vk::CommandBuffer &vkCommandBuffer) {
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
        mDevice->getGraphicsQueue().submit(submitInfos, fence);

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
    void Test05TextureImage::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
//        mVkLiteEngine.reset();
    }

} // test

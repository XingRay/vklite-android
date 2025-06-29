//
// Created by leixing on 2025/1/4.
//

#include "Test10NdkCameraFaceDetection.h"

#include "util/FileUtil.h"
#include "vklite/vklite_android.h"
#include "image/image.h"

/**
 * todo:
 * 1 先录制好所有命令, 在looper循环中执行即可
 * 2 compute pipeline 与 graphic pipeline 通过信号同步, 而不是 device.waitIdle()
 * 3 commandPool 提交一次性命令使用 semaphore 等待执行完成, 而不是等待 device.waitIdle()
 */
namespace test10 {

    Test10NdkCameraFaceDetection::Test10NdkCameraFaceDetection(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app), mNet() {

        // nn model
        int gpu_count = ncnn::get_gpu_count();
        if (gpu_count > 0) {
            LOG_D("use_vulkan_compute");
            ncnn::support_VK_KHR_surface = 1;
            ncnn::support_VK_KHR_android_surface = 1;

            mNcnnGpuDevice = std::make_unique<ncnn::VulkanDevice>(0);
            mBlobVkAllocator = std::unique_ptr<ncnn::VkAllocator>(mNcnnGpuDevice->acquire_blob_allocator());
            mStagingVkAllocator = std::unique_ptr<ncnn::VkAllocator>(mNcnnGpuDevice->acquire_staging_allocator());

            mNet.opt.blob_vkallocator = mBlobVkAllocator.get();
            mNet.opt.workspace_vkallocator = mBlobVkAllocator.get();
            mNet.opt.staging_vkallocator = mStagingVkAllocator.get();

            mNet.set_vulkan_device(mNcnnGpuDevice.get());

            mNcnnCompute = std::make_unique<ncnn::VkCompute>(mNcnnGpuDevice.get());


            VkInstance instance = ncnn::get_gpu_instance();
            LOG_D("instance: %p", instance);

            const ncnn::GpuInfo &gpuInfo = ncnn::get_gpu_info(0);
            VkPhysicalDevice physicalDevice = gpuInfo.physicalDevice();
            LOG_D("physicalDevice: %p", physicalDevice);

            VkDevice ncnnDevice = mNet.vulkan_device()->vkdevice();
            LOG_D("ncnnDevice: %p", ncnnDevice);

            mNet.opt.use_vulkan_compute = true;

            mNet.opt.use_fp16_packed = false;
            mNet.opt.use_fp16_storage = false;
            mNet.opt.use_fp16_arithmetic = false;
            mNet.opt.use_int8_storage = false;
            mNet.opt.use_int8_arithmetic = false;

            mNet.opt.use_shader_pack8 = true;
        }

        mNdkCamera = std::make_unique<ndkcamera::NdkCamera>();



//        mInstance = vklite::InstanceBuilder()
//                .addPlugin(vklite::AndroidSurfacePlugin::buildUniqueCombined())
//                .addPlugin(vklite::HardwareBufferPlugin::buildUnique())
//                .buildUnique();

        mInstance = std::make_unique<vklite::Instance>(ncnn::get_gpu_instance());
        vklite::VulkanAndroidApi::initApi(*mInstance);

        mSurface = vklite::AndroidSurfaceBuilder()
                .instance((*mInstance).getVkInstance())
                .nativeWindow(app.window)
                .buildUnique();

//        mPhysicalDevice = vklite::PhysicalDeviceSelector::makeDefault(*mSurface)
//                .selectUnique((*mInstance).enumeratePhysicalDevices());
        mPhysicalDevice = std::make_unique<vklite::PhysicalDevice>(ncnn::get_gpu_info(0).physicalDevice());

        vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
        if (mMsaaEnable) {
            sampleCount = vklite::MaxMsaaSampleCountSelector(4).select(mPhysicalDevice->querySampleCountFlagBits());
        }

        uint32_t presentQueueFamilyIndex = mPhysicalDevice->queryQueueFamilyIndicesBySurface(mSurface->getVkSurface())[0];
        uint32_t graphicAndComputeQueueFamilyIndex = mPhysicalDevice->queryQueueFamilyIndicesByFlags(vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute)[0];

//        mDevice = vklite::DeviceBuilder()
//                .addPlugin(vklite::AndroidSurfacePlugin::buildUniqueCombined())
//                .addPlugin(vklite::HardwareBufferPlugin::buildUnique())
//                .physicalDevice(mPhysicalDevice->getPhysicalDevice())
//                .addQueueFamily(graphicAndComputeQueueFamilyIndex)
//                .addQueueFamily(presentQueueFamilyIndex)
//                .buildUnique();
        mDevice = std::make_unique<vklite::Device>(mNet.vulkan_device()->vkdevice());
        LOG_D("device => %p", (void *) mDevice->getVkDevice());

        mGraphicQueue = std::make_unique<vklite::Queue>(mDevice->getQueue(graphicAndComputeQueueFamilyIndex));
        mComputeQueue = std::make_unique<vklite::Queue>(mDevice->getQueue(graphicAndComputeQueueFamilyIndex));
        mPresentQueue = std::make_unique<vklite::Queue>(mDevice->getQueue(presentQueueFamilyIndex));

        mSwapchain = vklite::SwapchainBuilder()
                .device(mDevice->getVkDevice())
                .surface(mSurface->getVkSurface())
                .queueFamilyIndices({presentQueueFamilyIndex})
                .config(mPhysicalDevice->getVkPhysicalDevice(), mSurface->getVkSurface())
                .buildUnique();

        mViewports = mSwapchain->fullScreenViewports();
        mScissors = mSwapchain->fullScreenScissors();

        mCommandPool = vklite::CommandPoolBuilder()
                .device(mDevice->getVkDevice())
                .queueFamilyIndex(graphicAndComputeQueueFamilyIndex)
                .buildUnique();
        mCommandBuffers = mCommandPool->allocateUnique(mFrameCount);
        mComputeCommandBuffers = mCommandPool->allocateUnique(mFrameCount);

        // 创建附件
        mDisplayImageViews = mSwapchain->createDisplayImageViews();

        mColorImageView = nullptr;
        if (mMsaaEnable) {
            mColorImageView = vklite::CombinedImageViewBuilder().asColorAttachment()
                    .device(mDevice->getVkDevice())
                    .format(mSwapchain->getVkFormat())
                    .size(mSwapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
                    .buildUnique();
        }

        mDepthImageView = nullptr;
        if (mDepthTestEnable) {
            mDepthImageView = vklite::CombinedImageViewBuilder().asDepthAttachment()
                    .device(mDevice->getVkDevice())
                    .format(mPhysicalDevice->findDepthFormat())
                    .size(mSwapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
                    .buildUnique();
        }

        vklite::Subpass externalSubpass = vklite::Subpass::externalSubpass();
        mRenderPass = vklite::RenderPassBuilder()
                .device(mDevice->getVkDevice())
                .renderAreaExtend(mSwapchain->getDisplaySize())
                .addSubpass([&](vklite::Subpass &subpass, const std::vector<vklite::Subpass> &subpasses) {
                    subpass
                            .pipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                            .addDependency(externalSubpass,
                                           vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                           vk::AccessFlagBits::eNone,
                                           vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                           vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);
                })
                .addSubpass([&](vklite::Subpass &subpass, const std::vector<vklite::Subpass> &subpasses) {
                    subpass
                            .pipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                            .addDependency(//externalSubpass,
                                    subpasses[0],
                                    vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                    vk::AccessFlagBits::eColorAttachmentWrite,
                                    vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                    vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);
                })
                .addSubpass([&](vklite::Subpass &subpass, const std::vector<vklite::Subpass> &subpasses) {
                    subpass
                            .pipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                            .addDependency(subpasses[1],
                                           vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                           vk::AccessFlagBits::eColorAttachmentWrite,
                                           vk::PipelineStageFlagBits::eFragmentShader,
                                           vk::AccessFlagBits::eInputAttachmentRead
                            );
                })
                .addAttachmentIf(mMsaaEnable, [&](vklite::Attachment &attachment, std::vector<vklite::Subpass> &subpasses) {
                    vklite::Attachment::msaaColorAttachment(attachment)
                            .sampleCount(sampleCount)
                            .format(mSwapchain->getVkFormat())
                            .clearColorValue(mClearColor)
                            .asColorAttachmentUsedIn(subpasses[0]);
                })
                .addAttachment([&](vklite::Attachment &attachment, std::vector<vklite::Subpass> &subpasses) {
                    vklite::Attachment::presentColorAttachment(attachment)
                            .format(mSwapchain->getVkFormat())
                            .clearColorValue(mClearColor)
                            .applyIf(mMsaaEnable, [&](vklite::Attachment &thiz) {
                                thiz
                                        .loadOp(vk::AttachmentLoadOp::eDontCare)
                                        .asResolveAttachmentUsedIn(subpasses[0]);
                            })
                            .applyIf(!mMsaaEnable, [&](vklite::Attachment &thiz) {
                                thiz
                                        .asColorAttachmentUsedIn(subpasses[0])
                                        .asInputAttachmentUsedIn(subpasses[1])
                                        .asColorAttachmentUsedIn(subpasses[1])
                                        .asInputAttachmentUsedIn(subpasses[2])
                                        .asColorAttachmentUsedIn(subpasses[2]);
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
                            .device(mDevice->getVkDevice())
                            .renderPass(mRenderPass->getVkRenderPass())
                            .size(mSwapchain->getDisplaySize())
                                    // 下面添加附件的顺序不能乱, 附件的顺序由 RenderPass 的附件定义顺序决定，必须严格一致。
                            .addAttachmentIf(mMsaaEnable, [&]() { return mColorImageView->getVkImageView(); })
                            .addAttachment(mDisplayImageViews[index].getVkImageView())
                            .addAttachmentIf(mDepthTestEnable, [&]() { return mDepthImageView->getVkImageView(); })
                            .build();
                })
                .build();

        mImageAvailableSemaphores = vklite::SemaphoreBuilder().device(mDevice->getVkDevice()).build(mFrameCount);
        mRenderFinishedSemaphores = vklite::SemaphoreBuilder().device(mDevice->getVkDevice()).build(mFrameCount);
        mFences = vklite::FenceBuilder()
                .device(mDevice->getVkDevice())
                        // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
                .fenceCreateFlags(vk::FenceCreateFlagBits::eSignaled)
                .build(mFrameCount);


        // preprocess
        mNdkCamera->startPreview();
        ndkcamera::Image image = mNdkCamera->loopAcquireImageWithBuffer();

        vklite::HardwareBuffer hardwareBuffer = vklite::HardwareBufferBuilder()
                .device(mDevice->getVkDevice())
                .hardwareBuffer(image.getHardwareBuffer())
                .build();

        mCameraInputSampler = vklite::CombinedHardwareBufferSamplerBuilder()
                .device(mDevice->getVkDevice())
                .formatProperties(hardwareBuffer.getFormatProperties())
                .buildUnique();

        std::vector<uint32_t> preprocessComputeShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/10_ndk_camera_face_detection_00_preprocess.comp.spv");
        vklite::ShaderConfigure preprocessComputeShaderConfigure = vklite::ShaderConfigure()
                .computeShaderCode(std::move(preprocessComputeShaderCode))
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addImmutableSampler(0, mCameraInputSampler->getVkSampler(), vk::ShaderStageFlagBits::eCompute)
                            .addUniformBuffer(1, vk::ShaderStageFlagBits::eCompute)
                            .addStorageBuffer(2, vk::ShaderStageFlagBits::eCompute)
//                            .addStorageImage(3, vk::ShaderStageFlagBits::eCompute)
                            ;
                });


        // preview
        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/10_ndk_camera_face_detection_01_preview.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/10_ndk_camera_face_detection_01_preview.frag.spv");

        vklite::ShaderConfigure previewShaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3)
                            .addAttribute(1, ShaderFormat::Vec2);
                })
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addImmutableSampler(0, mCameraInputSampler->getVkSampler(), vk::ShaderStageFlagBits::eFragment);
//                            .addCombinedImageSampler(0, vk::ShaderStageFlagBits::eFragment);
                });


        // lines
        std::vector<uint32_t> linesVertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/10_ndk_camera_face_detection_02_lines.vert.spv");
        std::vector<uint32_t> linesFragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/10_ndk_camera_face_detection_02_lines.frag.spv");

        vklite::ShaderConfigure linesShaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(linesVertexShaderCode))
                .fragmentShaderCode(std::move(linesFragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(SimpleVertex))
                            .addAttribute(0, ShaderFormat::Vec3);
                })
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniformBuffer(0, vk::ShaderStageFlagBits::eVertex);
                });


        // points
        std::vector<uint32_t> pointsVertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/10_ndk_camera_face_detection_03_points.vert.spv");
        std::vector<uint32_t> pointsFragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/10_ndk_camera_face_detection_03_points.frag.spv");

        vklite::ShaderConfigure pointsShaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(pointsVertexShaderCode))
                .fragmentShaderCode(std::move(pointsFragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(SimpleVertex))
                            .addAttribute(0, ShaderFormat::Vec3);
                })
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniformBuffer(0, vk::ShaderStageFlagBits::eVertex);
                });


        mDescriptorPool = vklite::DescriptorPoolBuilder()
                .device(mDevice->getVkDevice())
                .frameCount(mFrameCount)
                .addDescriptorPoolSizes(preprocessComputeShaderConfigure.calcDescriptorPoolSizes())
                .addDescriptorSetCount(preprocessComputeShaderConfigure.getDescriptorSetCount())
                .addDescriptorPoolSizes(previewShaderConfigure.calcDescriptorPoolSizes())
                .addDescriptorSetCount(previewShaderConfigure.getDescriptorSetCount())
                .addDescriptorPoolSizes(linesShaderConfigure.calcDescriptorPoolSizes())
                .addDescriptorSetCount(linesShaderConfigure.getDescriptorSetCount())
                .addDescriptorPoolSizes(pointsShaderConfigure.calcDescriptorPoolSizes())
                .addDescriptorSetCount(pointsShaderConfigure.getDescriptorSetCount())
                .buildUnique();


        // preprocess
        mPreprocessPipeline = vklite::CombinedComputePipelineBuilder()
                .device(mDevice->getVkDevice())
                .descriptorPool(mDescriptorPool->getVkDescriptorPool())
                .frameCount(mFrameCount)
                .shaderConfigure(preprocessComputeShaderConfigure)
                .buildUnique();

        mComputeFences = vklite::FenceBuilder()
                .device(mDevice->getVkDevice())
                        // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
                .fenceCreateFlags(vk::FenceCreateFlagBits::eSignaled)
                .build(mFrameCount);

        mComputeFinishSemaphores = vklite::SemaphoreBuilder()
                .device(mDevice->getVkDevice())
                .build(mFrameCount);

        // preview
        mPreviewPipeline = vklite::CombinedGraphicPipelineBuilder()
                .device(mDevice->getVkDevice())
                .descriptorPool(mDescriptorPool->getVkDescriptorPool())
                .frameCount(mFrameCount)
                .shaderConfigure(previewShaderConfigure)
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .renderPass(mRenderPass->getVkRenderPass(), 0)
                .viewports(mViewports)
                .scissors(mScissors)
                .buildUnique();

        // lines
        mLinesPipeline = vklite::CombinedGraphicPipelineBuilder()
                .device(mDevice->getVkDevice())
                .descriptorPool(mDescriptorPool->getVkDescriptorPool())
                .frameCount(mFrameCount)
                .shaderConfigure(linesShaderConfigure)
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .renderPass(mRenderPass->getVkRenderPass(), 1)
                .viewports(mViewports)
                .scissors(mScissors)
                .topology(vk::PrimitiveTopology::eLineList)
                .polygonMode(vk::PolygonMode::eLine)
                .lineWidth(1.0f)
                .buildUnique();

        // points
        mPointsPipeline = vklite::CombinedGraphicPipelineBuilder()
                .device(mDevice->getVkDevice())
                .descriptorPool(mDescriptorPool->getVkDescriptorPool())
                .frameCount(mFrameCount)
                .shaderConfigure(pointsShaderConfigure)
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .renderPass(mRenderPass->getVkRenderPass(), 2)
                .viewports(mViewports)
                .scissors(mScissors)
                .topology(vk::PrimitiveTopology::ePointList)
                .polygonMode(vk::PolygonMode::ePoint)
                .buildUnique();


        // nn model
        LOG_D("加载 param 文件");
        AAsset *modelParams = AAssetManager_open(mApp.activity->assetManager, "model/ncnn/face_detector/face_detector.param", AASSET_MODE_BUFFER);
        if (mNet.load_param(modelParams) != 0) {
            LOG_E("加载 param 文件失败");
            return;
        } else {
            LOG_D("加载 param 文件完成");
        }
        AAsset_close(modelParams);

        LOG_D("加载 bin 文件");
        AAsset *modelBin = AAssetManager_open(mApp.activity->assetManager, "model/ncnn/face_detector/face_detector.bin", AASSET_MODE_BUFFER);
        if (mNet.load_model(modelBin) != 0) {
            LOG_E("加载 bin 文件失败");
            return;
        } else {
            LOG_D("加载 bin 文件完成");
        }
        AAsset_close(modelBin);

        mExtractor = std::make_unique<ncnn::Extractor>(mNet.create_extractor());
        mExtractor->set_blob_vkallocator(mBlobVkAllocator.get());
        mExtractor->set_workspace_vkallocator(mBlobVkAllocator.get());
        mExtractor->set_staging_vkallocator(mStagingVkAllocator.get());
        mExtractor->set_vulkan_compute(true);
    }

    void Test10NdkCameraFaceDetection::init() {
        mAnchors = image_process::Anchor::generateAnchors();
        mLetterBox = image_process::LetterBox::calcLetterbox(1080, 1920, 128, 128);

        // preprocess pipeline

        // letter box params
        LetterboxParam letterboxParam{};
        float scale = mLetterBox.scale;
        letterboxParam.outputSize = {mLetterBox.targetWidth, mLetterBox.targetHeight};
        letterboxParam.unpaddingSize = {mLetterBox.paddedWidth, mLetterBox.paddedHeight};
        letterboxParam.padding = {mLetterBox.paddingLeft, mLetterBox.paddingTop};
        letterboxParam.fillColor = {0.0f, 0.0f, 0.0f, 1.0f};

        mLetterboxParamsUniformBuffers = vklite::UniformBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
                .addUsage(vk::BufferUsageFlagBits::eStorageBuffer)
                .size(sizeof(LetterboxParam))
                .build(mFrameCount);
        for (int i = 0; i < mFrameCount; i++) {
            mLetterboxParamsUniformBuffers[i].update(*mCommandPool, &letterboxParam, sizeof(LetterboxParam));
        }


//        // letter box output image
//        mLetterBoxOutputImageViews = vklite::CombinedImageViewBuilder()
//                .asStorageImage()
//                .device(mDevice->getVkDevice())
//                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
//                .format(vk::Format::eR32G32B32A32Sfloat)
//                .size(128, 128)
//                .build(mFrameCount);
//
//        for (int i = 0; i < mFrameCount; i++) {
//            mLetterBoxOutputImageViews[i].getImage().changeImageLayout(*mCommandPool,
//                                                                       vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral,
//                                                                       vk::PipelineStageFlagBits::eAllCommands, vk::PipelineStageFlagBits::eAllCommands,
//                                                                       vk::AccessFlagBits::eNone, vk::AccessFlagBits::eNone,
//                                                                       vk::ImageAspectFlagBits::eColor);
//        }

        for (int i = 0; i < mFrameCount; i++) {
            ncnn::VkMat mat;
            mat.create(128, 128, 3, (size_t) 4, 1, mBlobVkAllocator.get());
            mLetterBoxOutputNcnnBuffers.push_back(std::move(mat));
        }

//        for (int i = 0; i < mFrameCount; i++) {
//            ncnn::VkImageMat imageMat;
//            // 每个像素 rgba_f32 是 4*4=16 byte , 每个
//            imageMat.create(128, 128, (size_t) 16, 4, mBlobVkAllocator.get());
//            mLetterBoxOutputNcnnImages.push_back(std::move(imageMat));
//        }
//        for (int i = 0; i < mFrameCount; i++) {
//            vklite::PipelineBarrier pipelineBarrier = vklite::PipelineBarrierBuilder()
//                    .asDefault()
//                    .srcStage(vk::PipelineStageFlagBits::eAllCommands)
//                    .dstStage(vk::PipelineStageFlagBits::eAllCommands)
//                    .addImageMemoryBarrier([&](vklite::ImageMemoryBarrierBuilder &builder) {
//                        builder
//                                .asDefault()
//                                .image(mLetterBoxOutputNcnnImages[i].image())
//                                .oldLayout(vk::ImageLayout::eUndefined)
//                                .newLayout(vk::ImageLayout::eGeneral)
//                                .srcAccessMask(vk::AccessFlagBits::eNone)
//                                .dstAccessMask(vk::AccessFlagBits::eNone)
//                                .aspectMask(vk::ImageAspectFlagBits::eColor);
//                    })
//                    .build();
//            mCommandPool->submit([&](const vk::CommandBuffer &commandBuffer) {
//                pipelineBarrier.record(commandBuffer);
//            });
//        }

        vklite::DescriptorSetWriters preprocessDescriptorSetWriters = vklite::DescriptorSetWritersBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
                    configure
                            .descriptorSet(mPreprocessPipeline->getDescriptorSet(frameIndex, 0))
                            .addUniformBuffer([&](vklite::UniformBufferDescriptorMapping &mapping) {
                                mapping
                                        .binding(1)
                                        .addBufferInfo(mLetterboxParamsUniformBuffers[frameIndex].getBuffer());
                            })
                            .addStorageBuffer([&](vklite::StorageBufferDescriptorMapping &mapping) {
                                mapping
                                        .binding(2)
                                        .addBufferInfo(mLetterBoxOutputNcnnBuffers[frameIndex].buffer(), mLetterBoxOutputNcnnBuffers[frameIndex].buffer_offset(),
                                                       mLetterBoxOutputNcnnBuffers[frameIndex].buffer_capacity());
                            })
//                            .addStorageImage([&](vklite::StorageImageDescriptorMapping &mapping) {
//                                mapping
//                                        .binding(3)
////                                        .addImageInfo(mLetterBoxOutputImageViews[frameIndex].getImageView(), vk::ImageLayout::eGeneral);
//                                        .addImageInfo(mLetterBoxOutputNcnnImages[frameIndex].imageview(), vk::ImageLayout::eGeneral);
//                            })
                            ;
                })
                .build();

        mDevice->getVkDevice().updateDescriptorSets(preprocessDescriptorSetWriters.createWriteDescriptorSets(), nullptr);


        // preview pipeline // with uv rotation
//        std::vector<Vertex> vertices = {
//                {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}, // ndc 左上角 => uv 右下角
//                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f}}, // ndc 右上角 => uv 右上角
//                {{-1.0f, 1.0f,  0.0f}, {0.0f, 1.0f}}, // ndc 左下角 => uv 左下角
//                {{1.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}, // ndc 右下角 => uv 左上角
//        };

        // preview pipeline // no rotation
//        std::vector<Vertex> vertices = {
//                /*
//                 *   +  .
//                 *   .  .
//                 */
//                {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}, // ndc 左上角 => uv左上角
//
//                /*
//                 *   .  +
//                 *   .  .
//                 */
//                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f}}, // ndc右上角 => uv右上角
//
//                /*
//                 *   .  .
//                 *   .  +
//                 */
//                {{1.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // ndc右下角 => uv右下角
//
//                /*
//                 *   .  .
//                 *   +  .
//                 */
//                {{-1.0f, 1.0f,  0.0f}, {0.0f, 1.0f}}, // ndc左下角 => uv左下角
//
//        };

        std::vector<Vertex> vertices = {
                /*
                 *   +  .
                 *   .  .
                 */
                {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}, // ndc 左上角 => uv右下角

                /*
                 *   .  +
                 *   .  .
                 */
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f}}, // ndc右上角 => uv右上角

                /*
                 *   .  .
                 *   .  +
                 */
                {{1.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}, // ndc右下角 => uv左上角

                /*
                 *   .  .
                 *   +  .
                 */
                {{-1.0f, 1.0f,  0.0f}, {0.0f, 1.0f}}, // ndc左下角 => uv左下角

        };

        std::vector<uint32_t> indices = {
                0, 2, 1, 0, 3, 2,
        };

        uint32_t indicesSize = indices.size() * sizeof(uint32_t);
        mIndexBuffer = vklite::IndexBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
                .size(indicesSize)
                .buildUnique();
        mIndexBuffer->update(*mCommandPool, indices);
        mIndexVkBuffer = mIndexBuffer->getVkBuffer();
        mIndexCount = indices.size();


        uint32_t verticesSize = vertices.size() * sizeof(Vertex);
        mVertexBuffer = vklite::VertexBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
                .size(verticesSize)
                .buildUnique();
        mVertexBuffer->update(*mCommandPool, vertices.data(), verticesSize);
        mVertexBuffers.push_back((*mVertexBuffer).getVkBuffer());
        mVertexBufferOffsets.push_back(0);

        mPreprocessOutputImageSamplers = vklite::SamplerBuilder()
                .device(mDevice->getVkDevice())
                .build(mFrameCount);

//        vklite::DescriptorSetWriters previewDescriptorSetWriters = vklite::DescriptorSetWritersBuilder()
//                .frameCount(mFrameCount)
//                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
//                    configure
//                            .descriptorSet(mPreviewPipeline->getDescriptorSet(frameIndex, 0))
//                            .addCombinedImageSampler([&](vklite::CombinedImageSamplerDescriptorMapping &descriptorMapping) {
//                                descriptorMapping
//                                        .binding(0)
////                                        .addImageInfo(mPreprocessOutputImageSamplers[frameIndex], mLetterBoxOutputImageViews[frameIndex].getImageView());
//                                        .addImageInfo(mPreprocessOutputImageSamplers[frameIndex].getVkSampler(), mLetterBoxOutputNcnnImages[frameIndex].imageview());
//                            });
//                })
//                .build();
//
//        mDevice->getVkDevice().updateDescriptorSets(previewDescriptorSetWriters.createWriteDescriptorSets(), nullptr);


        // lines pipeline resources
        std::vector<SimpleVertex> linesVertices = {
                {{-0.5f, -0.5f, 0.0f}},  // 左上角
                {{0.5f,  -0.5f, 0.0f}},  // 右上角
                {{0.5f,  0.5f,  0.0f}},  // 右下角
                {{-0.5f, 0.5f,  0.0f}}   // 左下角
        };

        std::vector<uint32_t> linesIndices = {
                0, 1,  // 上边
                1, 2,  // 右边
                2, 3,  // 底边
                3, 0   // 左边
        };

        uint32_t linesIndicesSize = linesIndices.size() * sizeof(uint32_t);
        mLinesIndexBuffer = vklite::IndexBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
                .size(linesIndicesSize)
                .buildUnique();
        mLinesIndexBuffer->update(*mCommandPool, linesIndices);
        mLinesIndexVkBuffer = mLinesIndexBuffer->getVkBuffer();
        mLinesIndexCount = linesIndices.size();


        uint32_t linesVerticesSize = linesVertices.size() * sizeof(SimpleVertex);
        mLinesVertexBuffer = vklite::VertexBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
                .size(linesVerticesSize)
                .buildUnique();
        mLinesVertexBuffer->update(*mCommandPool, linesVertices.data(), linesVerticesSize);
        mLinesVertexBuffers.push_back((*mLinesVertexBuffer).getVkBuffer());
        mLinesVertexBufferOffsets.push_back(0);


        ColorUniformBufferObject colorUniformBufferObject{{0.0f, 0.0f, 1.0f}};
        mLinesUniformBuffers = vklite::UniformBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
                .size(sizeof(ColorUniformBufferObject))
                .build(mFrameCount);

        for (uint32_t i = 0; i < mFrameCount; i++) {
            mLinesUniformBuffers[i].update(*mCommandPool, &colorUniformBufferObject, sizeof(ColorUniformBufferObject));
        }

        vklite::DescriptorSetWriters linesDescriptorSetWriters = vklite::DescriptorSetWritersBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
                    configure
                            .descriptorSet(mLinesPipeline->getDescriptorSet(frameIndex, 0))
                            .addUniformBuffer([&](vklite::UniformBufferDescriptorMapping &mapping) {
                                mapping
                                        .addBufferInfo(mLinesUniformBuffers[frameIndex].getBuffer());
                            });
                })
                .build();

        mDevice->getVkDevice().updateDescriptorSets(linesDescriptorSetWriters.createWriteDescriptorSets(), nullptr);


        // points
        std::vector<SimpleVertex> pointsVertices = {
                {{-0.25f, -0.25f, 0.0f}},
                {{0.25f,  -0.25f, 0.0f}},
                {{0.25f,  0.25f,  0.0f}},
                {{-0.25f, 0.25f,  0.0f}},
                {{0.0f,   0.0f,   0.0f}},
                {{0.0f,   0.0f,   0.0f}},

        };

        uint32_t pointsVerticesSize = pointsVertices.size() * sizeof(SimpleVertex);
        mPointsVertexBuffer = vklite::VertexBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
                .size(pointsVerticesSize)
                .buildUnique();
        mPointsVertexBuffer->update(*mCommandPool, pointsVertices.data(), pointsVerticesSize);
        mPointsVertexBuffers.push_back((*mPointsVertexBuffer).getVkBuffer());
        mPointsVertexBufferOffsets.push_back(0);
        mPointsCount = pointsVertices.size();

        PointAttribute pointAttribute{{1.0f, 0.0f, 0.0f}, 14.0f};
        mPointsUniformBuffers = vklite::UniformBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties())
                .size(sizeof(PointAttribute))
                .build(mFrameCount);

        for (uint32_t i = 0; i < mFrameCount; i++) {
            mPointsUniformBuffers[i].update(*mCommandPool, &pointAttribute, sizeof(PointAttribute));
        }

        vklite::DescriptorSetWriters pointsDescriptorSetWriters = vklite::DescriptorSetWritersBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
                    configure
                            .descriptorSet(mPointsPipeline->getDescriptorSet(frameIndex, 0))
                            .addUniformBuffer([&](vklite::UniformBufferDescriptorMapping &mapping) {
                                mapping
                                        .addBufferInfo(mPointsUniformBuffers[frameIndex].getBuffer());
                            });
                })
                .build();

        mDevice->getVkDevice().updateDescriptorSets(pointsDescriptorSetWriters.createWriteDescriptorSets(), nullptr);

        mFrameCounter.start();

        LOG_D("Test10NdkCameraFaceDetection::init()");
    }

    // 检查是否准备好
    bool Test10NdkCameraFaceDetection::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test10NdkCameraFaceDetection::drawFrame() {

        // camera input
        std::optional<ndkcamera::Image> image = mNdkCamera->acquireLatestImage();
        if (!image.has_value()) {
//            LOG_D("Test07NdkCamera::drawFrame(), no image");
            return;
        }
        AHardwareBuffer *pHardwareBuffer = image.value().getHardwareBuffer();
        if (pHardwareBuffer == nullptr) {
            LOG_D("Test07NdkCamera::drawFrame(), no hardwareBuffer");
            return;
        }

        vklite::HardwareBuffer hardwareBuffer = vklite::HardwareBufferBuilder()
                .device(mDevice->getVkDevice())
                .hardwareBuffer(pHardwareBuffer)
                .build();

        mCameraInputImageView = vklite::CombinedHardwareBufferImageViewBuilder()
                .device(mDevice->getVkDevice())
                .hardwareBuffer(hardwareBuffer.getHardwareBuffer())
                .hardwareBufferFormatProperties(hardwareBuffer.getFormatProperties())
                .hardwareBufferDescription(hardwareBuffer.getAndroidHardwareBufferDescription())
                .hardwareBufferProperties(hardwareBuffer.getProperties())
                .memoryProperties(mPhysicalDevice->getMemoryProperties())
                .conversion((*mCameraInputSampler).getVkSamplerYcbcrConversion())
                .buildUnique();

        // bind to pipeline resources
        vklite::DescriptorSetWriters preprocessDescriptorSetWriters = vklite::DescriptorSetWritersBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
                    configure
                            .descriptorSet(mPreprocessPipeline->getDescriptorSet(frameIndex, 0))
                            .addMapping([&](vklite::DescriptorMapping &descriptorMapping) {
                                descriptorMapping
                                        .binding(0)
                                        .descriptorType(vk::DescriptorType::eCombinedImageSampler)
                                        .addImageInfo(mCameraInputSampler->getSampler(), mCameraInputImageView->getImageView());
                            });
                })
                .build();

        mDevice->getVkDevice().updateDescriptorSets(preprocessDescriptorSetWriters.createWriteDescriptorSets(), nullptr);

        vklite::DescriptorSetWriters previewDescriptorSetWriters = vklite::DescriptorSetWritersBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
                    configure
                            .descriptorSet(mPreviewPipeline->getDescriptorSet(frameIndex, 0))
                            .addCombinedImageSampler([&](vklite::CombinedImageSamplerDescriptorMapping &descriptorMapping) {
                                descriptorMapping
                                        .binding(0)
                                        .addImageInfo(mCameraInputSampler->getSampler(), mCameraInputImageView->getImageView());
                            });
                })
                .build();

        mDevice->getVkDevice().updateDescriptorSets(previewDescriptorSetWriters.createWriteDescriptorSets(), nullptr);

        // compute shader preprocess
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
//            mLetterBoxOutputImageViews[mCurrentFrameIndex].getImage().changeImageLayout(commandBuffer);

//            vklite::PipelineBarrier pipelineBarrier = vklite::PipelineBarrierBuilder()
//                    .asDefault()
//                    .srcStage(vk::PipelineStageFlagBits::eAllCommands)
//                    .dstStage(vk::PipelineStageFlagBits::eAllCommands)
//                    .addImageMemoryBarrier([&](vklite::ImageMemoryBarrierBuilder &builder) {
//                        builder
//                                .asDefault()
//                                .image(mLetterBoxOutputNcnnImages[mCurrentFrameIndex].image())
//                                .oldLayout(vk::ImageLayout::eUndefined)
//                                .newLayout(vk::ImageLayout::eGeneral)
//                                .srcAccessMask(vk::AccessFlagBits::eNone)
//                                .dstAccessMask(vk::AccessFlagBits::eNone)
//                                .aspectMask(vk::ImageAspectFlagBits::eColor);
//                    })
//                    .build();
//            pipelineBarrier.record(commandBuffer);

            commandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, mPreprocessPipeline->getVkPipeline());
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, mPreprocessPipeline->getVkPipelineLayout(), 0, mPreprocessPipeline->getDescriptorSets(mCurrentFrameIndex), nullptr);

            // todo: local_size_x local_size_y local_size_z
            commandBuffer.dispatch(128 / 32, 128 / 16, 1);
        });

        mComputeQueue->submit(computeCommandBuffer.getVkCommandBuffer(),
                              mComputeFinishSemaphores[mCurrentFrameIndex].getVkSemaphore(),
                              mComputeFences[mCurrentFrameIndex].getVkFence());

        result = mComputeFences[mCurrentFrameIndex].wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }


        ncnn::Extractor extractor = mNet.create_extractor();
        extractor.set_blob_vkallocator(mBlobVkAllocator.get());
        extractor.set_workspace_vkallocator(mBlobVkAllocator.get());
        extractor.set_staging_vkallocator(mStagingVkAllocator.get());

        ncnn::VkCompute ncnnCompute = ncnn::VkCompute(mNcnnGpuDevice.get());
        extractor.input("in0", mLetterBoxOutputNcnnBuffers[mCurrentFrameIndex]);

//        LOG_D("\n\n");
//        for (const ncnn::Blob &blob: mNet.blobs()) {
//            ncnn::VkMat blobOut;
//            extractor.extract(blob.name.c_str(), blobOut, ncnnCompute);
//            LOG_D("blobOut: %s, shape:[whdc: %d, %d, %d, %d], dims: %d, elemsize: %d, elempack: %d", blob.name.c_str(), blobOut.w, blobOut.h, blobOut.d, blobOut.c,
//                  blobOut.dims, blobOut.elempack, blobOut.elempack);
//        }
//        LOG_D("\n\n");


        ncnn::VkMat vkRegressors;
        extractor.extract("out0", vkRegressors, ncnnCompute);
        ncnn::VkMat vkScores;
        extractor.extract("out1", vkScores, ncnnCompute);

        ncnn::Mat packedRegressors;
        ncnnCompute.record_clone(vkRegressors, packedRegressors, mNet.opt);
        ncnn::Mat packedScores;
        ncnnCompute.record_clone(vkScores, packedScores, mNet.opt);

        ncnnCompute.submit_and_wait();
        ncnnCompute.reset();
        extractor.clear();

        ncnn::Mat regressors;
        ncnn::convert_packing(packedRegressors, regressors, 1, mNet.opt);
        ncnn::Mat scores;
        ncnn::convert_packing(packedScores, scores, 1, mNet.opt);


        int num_regressors = regressors.w * regressors.h * regressors.c; // 896*16
        int num_scores = scores.w * scores.h * scores.c; // 896
        float *regressors_data = (float *) regressors.data;
        float *scores_data = (float *) scores.data;

        std::vector<float> regressorsData(regressors_data, regressors_data + num_regressors);
        std::vector<float> scoreData(scores_data, scores_data + num_scores);

        // 对 score_vec 执行 clip(-100,100) 并计算 sigmoid
        for (auto &s: scoreData) {
            if (s < -100.0f) {
                s = -100.0f;
            }
            if (s > 100.0f) {
                s = 100.0f;
            }
//            s = 1.0f / (1.0f + std::exp(-s));
        }
        // 找到最大分数索引
        long maxIndex = std::distance(scoreData.begin(), std::max_element(scoreData.begin(), scoreData.end()));
        float max_score = scoreData[maxIndex];

        std::vector<float> bestRegressor(regressorsData.begin() + maxIndex * 16, regressorsData.begin() + maxIndex * 16 + 16);
        float box_dx = bestRegressor[0];
        float box_dy = bestRegressor[1];
        float w_box = bestRegressor[2];
        float h_box = bestRegressor[3];

        // 相对于 anchor 中心点的归一化偏移量
        std::vector<float> keyPoints(bestRegressor.begin() + 4, bestRegressor.end());
        // 检测框在输入图片(128*128)中的归一化坐标([0.0~1.0])
        const image_process::Anchor &anchor = mAnchors[maxIndex];

        // 计算边界框在输入图片(128*128)中的位置
        float box_center_x = box_dx + anchor.centerX * 128.0f;
        float box_center_y = box_dy + anchor.centerY * 128.0f;
        float box_w = w_box;
        float box_h = h_box;
        float box_x = box_center_x - box_w / 2.0f;
        float box_y = box_center_y - box_h / 2.0f;

        // 检测框的坐标
        cv::Rect box(cv::Point(std::round(box_x), std::round(box_y)),
                     cv::Size(std::round(box_w), std::round(box_h)));

        // 检测框在原图(1080*1920)中的坐标
        cv::Rect originalBox = mLetterBox.transformBack(box);
        float x0 = (originalBox.x / 1080.0f) * 2.0f - 1.0f;
        float y0 = (originalBox.y / 1920.0f) * 2.0f - 1.0f;
        float x1 = ((originalBox.x + originalBox.width) / 1080.0f) * 2.0f - 1.0f;
        float y1 = ((originalBox.y + originalBox.height) / 1920.0f) * 2.0f - 1.0f;

        std::vector<SimpleVertex> linesVertices;
        linesVertices.reserve(4);
        linesVertices.emplace_back(glm::vec3(x0, y0, 0.0f));
        linesVertices.emplace_back(glm::vec3(x1, y0, 0.0f));
        linesVertices.emplace_back(glm::vec3(x1, y1, 0.0f));
        linesVertices.emplace_back(glm::vec3(x0, y1, 0.0f));

        uint32_t linesVerticesSize = linesVertices.size() * sizeof(SimpleVertex);
        mLinesVertexBuffer->update(*mCommandPool, linesVertices.data(), linesVerticesSize);


        // 解析关键点在输入图片(128*128)中的坐标
        std::vector<cv::Point2f> kps;
        kps.reserve(keyPoints.size() / 2);
        for (size_t i = 0; i + 1 < keyPoints.size(); i += 2) {
            float kp_dx = keyPoints[i];
            float kp_dy = keyPoints[i + 1];
            float kp_x = kp_dx * anchor.width + anchor.centerX * 128.0f;
            float kp_y = kp_dy * anchor.height + anchor.centerY * 128.0f;
            kps.emplace_back(kp_x, kp_y);
        }

        // 关键点在原图(1080*1920)中的坐标
        std::vector<cv::Point> points = mLetterBox.transformBack(kps);

        std::vector<SimpleVertex> pointsVertices;
        pointsVertices.reserve(points.size());
        for (const cv::Point &point: points) {
            pointsVertices.emplace_back(glm::vec3((point.x / 1080.0f) * 2.0f - 1.0f, (point.y / 1920.0f) * 2.0f - 1.0f, 0.0f));
        }

        uint32_t pointsVerticesSize = pointsVertices.size() * sizeof(SimpleVertex);
        mPointsVertexBuffer->update(*mCommandPool, pointsVertices.data(), pointsVerticesSize);


        // graphic pipeline
        vklite::Semaphore &imageAvailableSemaphore = mImageAvailableSemaphores[mCurrentFrameIndex];
        vklite::Semaphore &renderFinishedSemaphore = mRenderFinishedSemaphores[mCurrentFrameIndex];
        vklite::Fence &fence = mFences[mCurrentFrameIndex];

        result = mFences[mCurrentFrameIndex].wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
        auto [acquireResult, imageIndex] = mSwapchain->acquireNextImage(imageAvailableSemaphore.getVkSemaphore());
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
            mRenderPass->execute(vkCommandBuffer, mFramebuffers[imageIndex].getVkFramebuffer(), [&](const vk::CommandBuffer &vkCommandBuffer) {
//                mLetterBoxOutputImageViews[mCurrentFrameIndex].getImage().changeImageLayout(vkCommandBuffer,
//                                                                                            vk::ImageLayout::eGeneral, vk::ImageLayout::eGeneral,
//                                                                                            vk::PipelineStageFlagBits::eComputeShader, vk::PipelineStageFlagBits::eFragmentShader,
//                                                                                            vk::AccessFlagBits::eShaderWrite, vk::AccessFlagBits::eShaderRead,
//                                                                                            vk::ImageAspectFlagBits::eColor);

//                vklite::PipelineBarrier pipelineBarrier = vklite::PipelineBarrierBuilder()
//                        .asDefault()
//                        .srcStage(vk::PipelineStageFlagBits::eComputeShader)
//                        .dstStage(vk::PipelineStageFlagBits::eFragmentShader)
//                        .addImageMemoryBarrier([&](vklite::ImageMemoryBarrierBuilder &builder) {
//                            builder
//                                    .asDefault()
//                                    .image(mLetterBoxOutputNcnnImages[mCurrentFrameIndex].image())
//                                    .oldLayout(vk::ImageLayout::eGeneral)
//                                    .newLayout(vk::ImageLayout::eGeneral)
//                                    .srcAccessMask(vk::AccessFlagBits::eShaderWrite)
//                                    .dstAccessMask(vk::AccessFlagBits::eShaderRead)
//                                    .aspectMask(vk::ImageAspectFlagBits::eColor);
//                        })
//                        .build();
//                pipelineBarrier.record(vkCommandBuffer);

                vkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPreviewPipeline->getVkPipeline());
                vkCommandBuffer.setViewport(0, mViewports);
                vkCommandBuffer.setScissor(0, mScissors);

                if (!mPreviewPipeline->getDescriptorSets().empty()) {
                    vkCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPreviewPipeline->getVkPipelineLayout(), 0, mPreviewPipeline->getDescriptorSets()[mCurrentFrameIndex],
                                                       nullptr);
                }

                for (const vklite::PushConstant &pushConstant: mPreviewPipeline->getPushConstants()) {
                    vkCommandBuffer.pushConstants(mPreviewPipeline->getVkPipelineLayout(),
                                                  pushConstant.getStageFlags(),
                                                  pushConstant.getOffset(),
                                                  pushConstant.getSize(),
                                                  pushConstant.getData());
                }

                vkCommandBuffer.bindVertexBuffers(0, mVertexBuffers, mVertexBufferOffsets);
                vkCommandBuffer.bindIndexBuffer(mIndexVkBuffer, 0, vk::IndexType::eUint32);
                vkCommandBuffer.drawIndexed(mIndexCount, 1, 0, 0, 0);


                vkCommandBuffer.nextSubpass(vk::SubpassContents::eInline);



                // lines
                vkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mLinesPipeline->getVkPipeline());
                vkCommandBuffer.setViewport(0, mViewports);
                vkCommandBuffer.setScissor(0, mScissors);
//                vkCommandBuffer.setLineWidth(8.0f); //todo

                if (!mLinesPipeline->getDescriptorSets().empty()) {
                    vkCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mLinesPipeline->getVkPipelineLayout(), 0, mLinesPipeline->getDescriptorSets()[mCurrentFrameIndex], nullptr);
                }

                for (const vklite::PushConstant &pushConstant: mLinesPipeline->getPushConstants()) {
                    vkCommandBuffer.pushConstants(mLinesPipeline->getVkPipelineLayout(),
                                                  pushConstant.getStageFlags(),
                                                  pushConstant.getOffset(),
                                                  pushConstant.getSize(),
                                                  pushConstant.getData());
                }

                vkCommandBuffer.bindVertexBuffers(0, mLinesVertexBuffers, mLinesVertexBufferOffsets);
                vkCommandBuffer.bindIndexBuffer(mLinesIndexVkBuffer, 0, vk::IndexType::eUint32);
                vkCommandBuffer.drawIndexed(mLinesIndexCount, 1, 0, 0, 0);


                vkCommandBuffer.nextSubpass(vk::SubpassContents::eInline);



                // points
                vkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPointsPipeline->getVkPipeline());
                vkCommandBuffer.setViewport(0, mViewports);
                vkCommandBuffer.setScissor(0, mScissors);

                if (!mPointsPipeline->getDescriptorSets().empty()) {
                    vkCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPointsPipeline->getVkPipelineLayout(), 0, mPointsPipeline->getDescriptorSets()[mCurrentFrameIndex], nullptr);
                }

                for (const vklite::PushConstant &pushConstant: mPointsPipeline->getPushConstants()) {
                    vkCommandBuffer.pushConstants(mPointsPipeline->getVkPipelineLayout(),
                                                  pushConstant.getStageFlags(),
                                                  pushConstant.getOffset(),
                                                  pushConstant.getSize(),
                                                  pushConstant.getData());
                }

                vkCommandBuffer.bindVertexBuffers(0, mPointsVertexBuffers, mPointsVertexBufferOffsets);
                vkCommandBuffer.draw(mPointsCount, 1, 0, 0);
            });
        });

        result = mFences[mCurrentFrameIndex].reset();
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("resetFences failed");
        }

        mGraphicQueue->submit(commandBuffer.getVkCommandBuffer(),
                              vk::PipelineStageFlagBits::eColorAttachmentOutput,
                              imageAvailableSemaphore.getVkSemaphore(),
                              renderFinishedSemaphore.getVkSemaphore(),
                              fence.getVkFence());

        result = mPresentQueue->present(mSwapchain->getVkSwapChain(), imageIndex, renderFinishedSemaphore.getVkSemaphore());
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


        mFrameCounter.count();
        if (mFrameCounter.getAndResetIsFpsUpdated()) {
            LOG_D("fps: %f", mFrameCounter.getFps());
        }
    }

    // 清理操作
    void Test10NdkCameraFaceDetection::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test
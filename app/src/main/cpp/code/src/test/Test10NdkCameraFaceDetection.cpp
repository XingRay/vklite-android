//
// Created by leixing on 2025/1/4.
//

#include "Test10NdkCameraFaceDetection.h"

#include "util/FileUtil.h"
#include "vklite/vklite_android.h"
#include "image/image.h"


namespace test10 {

    Test10NdkCameraFaceDetection::Test10NdkCameraFaceDetection(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app), mNet() {

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/09_face_image_detection.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/09_face_image_detection.frag.spv");

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
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addSampler(0, vk::ShaderStageFlagBits::eFragment);
                });

        std::vector<uint32_t> linesVertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/09_face_image_detection_02_lines.vert.spv");
        std::vector<uint32_t> linesFragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/09_face_image_detection_02_lines.frag.spv");

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
                            .addUniform(0, vk::ShaderStageFlagBits::eVertex);
                });

        std::vector<uint32_t> pointsVertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/09_face_image_detection_03_points.vert.spv");
        std::vector<uint32_t> pointsFragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/09_face_image_detection_03_points.frag.spv");

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
                            .addUniform(0, vk::ShaderStageFlagBits::eVertex);
                });

        mInstance = vklite::InstanceBuilder()
                .addPlugin(vklite::AndroidSurfacePlugin::buildUniqueCombined())
                .buildUnique();

        mSurface = vklite::AndroidSurfaceBuilder()
                .instance((*mInstance).getInstance())
                .nativeWindow(app.window)
                .buildUnique();

        mPhysicalDevice = vklite::PhysicalDeviceSelector::makeDefault(*mSurface)
                .selectUnique((*mInstance).enumeratePhysicalDevices());

        vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
        if (mMsaaEnable) {
            sampleCount = vklite::MaxMsaaSampleCountSelector(4).select(mPhysicalDevice->querySampleCountFlagBits());
        }

        uint32_t presentQueueFamilyIndex = mPhysicalDevice->queryQueueFamilyIndicesBySurface(mSurface->getSurface())[0];
        uint32_t graphicQueueFamilyIndex = mPhysicalDevice->queryQueueFamilyIndicesByFlags(vk::QueueFlagBits::eGraphics)[0];

        mDevice = vklite::DeviceBuilder()
                .addPlugin(vklite::AndroidSurfacePlugin::buildUniqueCombined())
                .physicalDevice(mPhysicalDevice->getPhysicalDevice())
                .addQueueFamily(graphicQueueFamilyIndex)
                .addQueueFamily(presentQueueFamilyIndex)
                .buildUnique();
        LOG_D("device => %p", (void *) mDevice->getDevice());

        mGraphicQueue = std::make_unique<vklite::Queue>(mDevice->getQueue(graphicQueueFamilyIndex));
        mPresentQueue = std::make_unique<vklite::Queue>(mDevice->getQueue(presentQueueFamilyIndex));

        mSwapchain = vklite::SwapchainBuilder()
                .device(mDevice->getDevice())
                .surface(mSurface->getSurface())
                .queueFamilyIndices({presentQueueFamilyIndex})
                .config(mPhysicalDevice->getPhysicalDevice(), mSurface->getSurface())
                .buildUnique();

        mViewports = mSwapchain->fullScreenViewports();
        mScissors = mSwapchain->fullScreenScissors();

        mCommandPool = vklite::CommandPoolBuilder()
                .device(mDevice->getDevice())
                .queueFamilyIndex(graphicQueueFamilyIndex)
                .buildUnique();
        mCommandBuffers = mCommandPool->allocateUnique(mFrameCount);

        // 创建附件
        mDisplayImageViews = mSwapchain->createDisplayImageViews();

        mColorImageView = nullptr;
        if (mMsaaEnable) {
            mColorImageView = vklite::CombinedImageViewBuilder().asColor()
                    .device(mDevice->getDevice())
                    .format(mSwapchain->getDisplayFormat())
                    .size(mSwapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .configDeviceMemory(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
                    .buildUnique();
        }

        mDepthImageView = nullptr;
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
                .addSubpass([&](vklite::Subpass &subpass, const std::vector<vklite::Subpass> &subpasses) {
                    subpass
                            .pipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                            .addDependency(externalSubpass,
                                    //vk::PipelineStageFlagBits::eBottomOfPipe,
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
//                                           vk::AccessFlagBits::eNone,

                                    vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                    vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);
//                                           vk::PipelineStageFlagBits::eFragmentShader,
//                                           vk::AccessFlagBits::eInputAttachmentRead);
                })
                .addSubpass([&](vklite::Subpass &subpass, const std::vector<vklite::Subpass> &subpasses) {
                    subpass
                            .pipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                            .addDependency(subpasses[1],
                                           vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                           vk::AccessFlagBits::eColorAttachmentWrite,
//                                           vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
//                                           vk::AccessFlagBits::eColorAttachmentWrite,

                                           vk::PipelineStageFlagBits::eFragmentShader,
                                           vk::AccessFlagBits::eInputAttachmentRead
//                                           vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
//                                           vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite
                            );
                })
                .addAttachmentIf(mMsaaEnable, [&](vklite::Attachment &attachment, std::vector<vklite::Subpass> &subpasses) {
                    vklite::Attachment::msaaColorAttachment(attachment)
                            .sampleCount(sampleCount)
                            .format(mSwapchain->getDisplayFormat())
                            .clearColorValue(mClearColor)
                            .asColorAttachmentUsedIn(subpasses[0]);
                })
                .addAttachment([&](vklite::Attachment &attachment, std::vector<vklite::Subpass> &subpasses) {
                    vklite::Attachment::presentColorAttachment(attachment)
                            .format(mSwapchain->getDisplayFormat())
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
                            .device(mDevice->getDevice())
                            .renderPass(mRenderPass->getRenderPass())
                            .size(mSwapchain->getDisplaySize())
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

        mDescriptorPool = vklite::DescriptorPoolBuilder()
                .device(mDevice->getDevice())
                .frameCount(mFrameCount)
                .addDescriptorPoolSizes(shaderConfigure.calcDescriptorPoolSizes())
                .addDescriptorSetCount(shaderConfigure.getDescriptorSetCount())
                .addDescriptorPoolSizes(linesShaderConfigure.calcDescriptorPoolSizes())
                .addDescriptorSetCount(linesShaderConfigure.getDescriptorSetCount())
                .addDescriptorPoolSizes(pointsShaderConfigure.calcDescriptorPoolSizes())
                .addDescriptorSetCount(pointsShaderConfigure.getDescriptorSetCount())
                .buildUnique();

        mPipeline = vklite::CombinedGraphicPipelineBuilder()
                .device(mDevice->getDevice())
                .descriptorPool(mDescriptorPool->getDescriptorPool())
                .frameCount(mFrameCount)
                .shaderConfigure(shaderConfigure)
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .renderPass(mRenderPass->getRenderPass(), 0)
                .viewports(mViewports)
                .scissors(mScissors)
                .buildUnique();

        mLinesPipeline = vklite::CombinedGraphicPipelineBuilder()
                .device(mDevice->getDevice())
                .descriptorPool(mDescriptorPool->getDescriptorPool())
                .frameCount(mFrameCount)
                .shaderConfigure(linesShaderConfigure)
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .renderPass(mRenderPass->getRenderPass(), 1)
                .viewports(mViewports)
                .scissors(mScissors)
                .topology(vk::PrimitiveTopology::eLineList)
                .polygonMode(vk::PolygonMode::eLine)
//                .cullMode(vk::CullModeFlagBits::eNone)
                .lineWidth(1.0f)
                .buildUnique();

        mPointsPipeline = vklite::CombinedGraphicPipelineBuilder()
                .device(mDevice->getDevice())
                .descriptorPool(mDescriptorPool->getDescriptorPool())
                .frameCount(mFrameCount)
                .shaderConfigure(pointsShaderConfigure)
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .renderPass(mRenderPass->getRenderPass(), 2)
                .viewports(mViewports)
                .scissors(mScissors)
                .topology(vk::PrimitiveTopology::ePointList)
                .polygonMode(vk::PolygonMode::ePoint)
                .buildUnique();
    }

    void Test10NdkCameraFaceDetection::init() {

        mNdkCamera = std::make_unique<ndkcamera::NdkCamera>();
        mNdkCamera->startPreview();

        ndkcamera::Image image = mNdkCamera->acquireLatestImageWithBuffer();
        vklite::HardwareBuffer hardwareBuffer = vklite::HardwareBuffer::build(mDevice, image.getHardwareBuffer());

//        AAsset *image = AAssetManager_open(mApp.activity->assetManager, "test/image/test_face_image_1080_1920_01.png", AASSET_MODE_BUFFER);
//        const uint8_t *data = static_cast<const uint8_t *>(AAsset_getBuffer(image));
//        LOG_D("AAsset_getLength(image): %ld", AAsset_getLength(image));
//        std::vector<uint8_t> buffer(data, data + AAsset_getLength(image));
//        cv::Mat imageMat = cv::imdecode(buffer, cv::IMREAD_COLOR); // 内存解码
//        AAsset_close(image);

        const char *imageFilePath = "/storage/emulated/0/test/face_image/face_image_04.png";
        cv::Mat imageMat = cv::imread(imageFilePath);

        LOG_DF("imageMat: cols:{}, rows:{}, depth:{}, channels:{}, dimensions:{}", imageMat.cols, imageMat.rows, imageMat.depth(), imageMat.channels(), imageMat.dims);
        if (imageMat.channels() == 4) {
            cv::cvtColor(imageMat, mImageMat, cv::COLOR_BGRA2RGB);
        } else {
            cv::cvtColor(imageMat, mImageMat, cv::COLOR_BGR2RGB);
        }

        mAnchors = image_process::Anchor::generateAnchors();
        mLetterBox = image_process::LetterBox::calcLetterbox(imageMat.cols, imageMat.rows, 128, 128);
        cv::Mat padded = mLetterBox.copyMakeBorder(imageMat);
        mMatIn = ncnn::Mat::from_pixels(padded.data, ncnn::Mat::PIXEL_RGB, padded.cols, padded.rows);
        const float norm_vals[3] = {1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f};
        const float mean_vals[3] = {0.0f, 0.0f, 0.0f};
        mMatIn.substract_mean_normalize(mean_vals, norm_vals);

        int gpu_count = ncnn::get_gpu_count();
        if (gpu_count > 0) {
            LOG_D("use_vulkan_compute");
            mNet.opt.use_vulkan_compute = true;

            // set specified vulkan device before loading param and model
            mNet.set_vulkan_device(0); // use device-0

            mNet.opt.use_fp16_packed = false;
            mNet.opt.use_fp16_storage = false;
            mNet.opt.use_fp16_arithmetic = false;
            mNet.opt.use_int8_storage = false;
            mNet.opt.use_int8_arithmetic = false;

            mNet.opt.use_shader_pack8 = true;
        }

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


        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},  // 左上
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f}},  // 右上
                {{-1.0f, 1.0f,  0.0f}, {0.0f, 1.0f}},  // 左下
                {{1.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},  // 右下

        };

        std::vector<uint32_t> indices = {
                0, 2, 1, 1, 2, 3,
        };

        uint32_t indicesSize = indices.size() * sizeof(uint32_t);
        mIndexBuffer = vklite::IndexBufferBuilder()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
                .size(indicesSize)
                .buildUnique();
        mIndexBuffer->update(*mCommandPool, indices);
        mIndexVkBuffer = mIndexBuffer->getVkBuffer();
        mIndexCount = indices.size();


        uint32_t verticesSize = vertices.size() * sizeof(Vertex);
        mVertexBuffer = vklite::VertexBufferBuilder()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
                .size(verticesSize)
                .buildUnique();
        mVertexBuffer->update(*mCommandPool, vertices.data(), verticesSize);
        mVertexBuffers.push_back((*mVertexBuffer).getVkBuffer());
        mVertexBufferOffsets.push_back(0);


        mSamplers = vklite::CombinedImageSamplerBuilder().asDefault()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
                .width(mImageMat.cols)
                .height(mImageMat.rows)
                .format(vk::Format::eR8G8B8Srgb)
                .build(mFrameCount);

        for (uint32_t i = 0; i < mFrameCount; i++) {
            mSamplers[i].getImage().transitionImageLayout(*mCommandPool);
            mSamplers[i].update(*mCommandPool, mImageMat.data, mImageMat.total() * mImageMat.elemSize());
        }

        vklite::DescriptorSetWriters descriptorSetWriters = vklite::DescriptorSetWritersBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
                    configure
                            .descriptorSet(mPipeline->getDescriptorSet(frameIndex, 0))
                            .addSampler([&](vklite::SamplerDescriptorMapping &mapping) {
                                mapping
                                        .addImageInfo(mSamplers[frameIndex].getSampler(), mSamplers[frameIndex].getImageView());
                            });
                })
                .build();

        std::vector<vk::WriteDescriptorSet> writeDescriptorSets = descriptorSetWriters.createWriteDescriptorSets();
        mDevice->getDevice().updateDescriptorSets(writeDescriptorSets, nullptr);



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
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
                .size(linesIndicesSize)
                .buildUnique();
        mLinesIndexBuffer->update(*mCommandPool, linesIndices);
        mLinesIndexVkBuffer = mLinesIndexBuffer->getVkBuffer();
        mLinesIndexCount = linesIndices.size();


        uint32_t linesVerticesSize = linesVertices.size() * sizeof(SimpleVertex);
        mLinesVertexBuffer = vklite::VertexBufferBuilder()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
                .size(linesVerticesSize)
                .buildUnique();
        mLinesVertexBuffer->update(*mCommandPool, linesVertices.data(), linesVerticesSize);
        mLinesVertexBuffers.push_back((*mLinesVertexBuffer).getVkBuffer());
        mLinesVertexBufferOffsets.push_back(0);


        ColorUniformBufferObject colorUniformBufferObject{{0.0f, 0.0f, 1.0f}};
        mLinesUniformBuffers = vklite::UniformBufferBuilder()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
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
                            .addUniform([&](vklite::UniformDescriptorMapping &mapping) {
                                mapping
                                        .addBufferInfo(mLinesUniformBuffers[frameIndex].getBuffer());
                            });
                })
                .build();

        mDevice->getDevice().updateDescriptorSets(linesDescriptorSetWriters.createWriteDescriptorSets(), nullptr);


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
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
                .size(pointsVerticesSize)
                .buildUnique();
        mPointsVertexBuffer->update(*mCommandPool, pointsVertices.data(), pointsVerticesSize);
        mPointsVertexBuffers.push_back((*mPointsVertexBuffer).getVkBuffer());
        mPointsVertexBufferOffsets.push_back(0);
        mPointsCount = pointsVertices.size();

        PointAttribute pointAttribute{{1.0f, 0.0f, 0.0f}, 14.0f};
        mPointsUniformBuffers = vklite::UniformBufferBuilder()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties())
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
                            .addUniform([&](vklite::UniformDescriptorMapping &mapping) {
                                mapping
                                        .addBufferInfo(mPointsUniformBuffers[frameIndex].getBuffer());
                            });
                })
                .build();

        mDevice->getDevice().updateDescriptorSets(pointsDescriptorSetWriters.createWriteDescriptorSets(), nullptr);

        mFrameCounter.start();

        LOG_D("Test10NdkCameraFaceDetection::init()");
    }

    // 检查是否准备好
    bool Test10NdkCameraFaceDetection::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test10NdkCameraFaceDetection::drawFrame() {

        mExtractor->input("in0", mMatIn);

        ncnn::Mat regressors;
        ncnn::Mat scores;
        mExtractor->extract("out0", regressors);
        mExtractor->extract("out1", scores);

        int num_regressors = regressors.w * regressors.h * regressors.c; // 896*16
        int num_scores = scores.w * scores.h * scores.c; // 896
        float *regressors_data = (float *) regressors.data;
        float *scores_data = (float *) scores.data;

        std::vector<float> reg_vec(regressors_data, regressors_data + num_regressors);
        std::vector<float> score_vec(scores_data, scores_data + num_scores);

        // 对 score_vec 执行 clip(-100,100) 并计算 sigmoid
        for (auto &s: score_vec) {
            if (s < -100.0f) {
                s = -100.0f;
            }
            if (s > 100.0f) {
                s = 100.0f;
            }
            s = 1.0f / (1.0f + std::exp(-s));
        }
        // 找到最大分数索引
        int maxIndex = std::distance(score_vec.begin(), std::max_element(score_vec.begin(), score_vec.end()));
        float max_score = score_vec[maxIndex];

        std::vector<float> bestRegressor(reg_vec.begin() + maxIndex * 16, reg_vec.begin() + maxIndex * 16 + 16);
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
                vkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline->getVkPipeline());
                vkCommandBuffer.setViewport(0, mViewports);
                vkCommandBuffer.setScissor(0, mScissors);

                if (!mPipeline->getDescriptorSets().empty()) {
                    vkCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipeline->getVkPipelineLayout(), 0, mPipeline->getDescriptorSets()[mCurrentFrameIndex], nullptr);
                }

                for (const vklite::PushConstant &pushConstant: mPipeline->getPushConstants()) {
                    vkCommandBuffer.pushConstants(mPipeline->getVkPipelineLayout(),
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
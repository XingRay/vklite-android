//
// Created by leixing on 2025/1/4.
//

#pragma once

#include <cstdint>

#include "vklite/Log.h"
#include "test/common/TestBase.h"

#include "game_activity/native_app_glue/android_native_app_glue.h"
#include "math/glm.h"

#include "vklite/vklite.h"
#include "vklite/vklite_android.h"
#include "engine/engines.h"
#include "util/FrameCounter.h"

#include <opencv2/opencv.hpp>
#include <ncnn/mat.h>
#include <ncnn/net.h>
#include "image_process/image_process.h"
#include "ndk_camera/NdkCamera.h"

namespace test10 {

    struct LetterboxParam {
        glm::ivec2 unpaddingSize;// 输出图像不包含 padding 部分的长宽
        glm::ivec2 padding;        // padding: left, top
        glm::vec4 fillColor;      // 填充颜色 (RGBA)
    };

    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;

        bool operator==(const Vertex &other) const {
            return position == other.position && uv == other.uv;
        }
    };

    struct SimpleVertex {
        glm::vec3 position;

        bool operator==(const SimpleVertex &other) const {
            return position == other.position;
        }
    };

    struct ColorUniformBufferObject {
        // 颜色数据
        alignas(16) glm::vec3 color;
    };

    struct PointAttribute {
        // 颜色数据
        alignas(16) glm::vec3 color;
        alignas(4) float size;
    };

    class Test10NdkCameraFaceDetection : public test::TestBase {
    private:

        const android_app &mApp;

        // config
        bool mMsaaEnable = false;
        bool mDepthTestEnable = false;
        std::array<float, 4> mClearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        float mClearDepth = 1.0f;
        uint32_t mFrameCount = 2;
        vk::SampleCountFlagBits mSampleCount;

        // vulkan instance
        std::unique_ptr<vklite::Instance> mInstance;
        std::unique_ptr<vklite::Surface> mSurface;
        std::unique_ptr<vklite::PhysicalDevice> mPhysicalDevice;

        std::unique_ptr<vklite::Device> mDevice;

        std::unique_ptr<vklite::Queue> mGraphicQueue;
        std::unique_ptr<vklite::Queue> mComputeQueue;
        std::unique_ptr<vklite::Queue> mPresentQueue;

        std::unique_ptr<vklite::Swapchain> mSwapchain;
        std::vector<vklite::ImageView> mDisplayImageViews;
        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;

        std::unique_ptr<vklite::CommandPool> mCommandPool;
        std::unique_ptr<vklite::CommandBuffers> mCommandBuffers;
        std::unique_ptr<vklite::CommandBuffers> mComputeCommandBuffers;

        std::unique_ptr<vklite::RenderPass> mRenderPass;
        std::unique_ptr<vklite::CombinedImageView> mColorImageView;
        std::unique_ptr<vklite::CombinedImageView> mDepthImageView;
        vklite::Framebuffers mFramebuffers;

        std::vector<vklite::Semaphore> mImageAvailableSemaphores;
        std::vector<vklite::Semaphore> mRenderFinishedSemaphores;
        std::vector<vklite::Fence> mFences;


        std::unique_ptr<vklite::DescriptorPool> mDescriptorPool;


        //status
        uint32_t mCurrentFrameIndex = 0;
        bool mFramebufferResized = false;


        // image preprocess compute pipeline
        std::unique_ptr<vklite::CombinedPipeline> mPreprocessPipeline;
        std::vector<vklite::Fence> mComputeFences;
        std::vector<vklite::Semaphore> mComputeFinishSemaphores;

        std::unique_ptr<vklite::CombinedHardwareBufferSampler> mCameraInputSampler;
        std::unique_ptr<vklite::CombinedHardwareBufferImageView> mCameraInputImageView;

        std::vector<vklite::CombinedImageView> mPreprocessOutputImageViews;
        std::vector<vklite::UniformBuffer> mLetterboxParamsUniformBuffers;


        //image pipeline
        std::unique_ptr<vklite::CombinedPipeline> mPreviewPipeline;
        // vertex buffer
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;
        std::vector<vk::Buffer> mVertexBuffers;
        std::vector<vk::DeviceSize> mVertexBufferOffsets;

        // index buffer
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;
        vk::Buffer mIndexVkBuffer;
        uint32_t mIndexCount;

        std::vector<vklite::Sampler> mPreprocessOutputImageSamplers;


        // lines pipeline resources
        std::unique_ptr<vklite::CombinedPipeline> mLinesPipeline;
        // vertex buffer
        std::unique_ptr<vklite::VertexBuffer> mLinesVertexBuffer;
        std::vector<vk::Buffer> mLinesVertexBuffers;
        std::vector<vk::DeviceSize> mLinesVertexBufferOffsets;

        // index buffer
        std::unique_ptr<vklite::IndexBuffer> mLinesIndexBuffer;
        vk::Buffer mLinesIndexVkBuffer;
        uint32_t mLinesIndexCount;

        std::vector<vklite::UniformBuffer> mLinesUniformBuffers;


        // points pipeline resources
        std::unique_ptr<vklite::CombinedPipeline> mPointsPipeline;
        // vertex buffer
        std::unique_ptr<vklite::VertexBuffer> mPointsVertexBuffer;
        std::vector<vk::Buffer> mPointsVertexBuffers;
        std::vector<vk::DeviceSize> mPointsVertexBufferOffsets;
        uint32_t mPointsCount;

        std::vector<vklite::UniformBuffer> mPointsUniformBuffers;


        // nn model
        std::vector<image_process::Anchor> mAnchors;
        image_process::LetterBox mLetterBox;

        ncnn::Net mNet;
        std::unique_ptr<ncnn::Extractor> mExtractor;

        std::unique_ptr<ndkcamera::NdkCamera> mNdkCamera;

        util::FrameCounter mFrameCounter;

    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test10NdkCameraFaceDetection(const android_app &app, const std::string &name);

        // 初始化操作
        void init() override;

        // 检查是否准备好
        bool isReady() override;

        // 绘制三角形帧
        void drawFrame() override;

        // 清理操作
        void cleanup() override;
    };

} // test

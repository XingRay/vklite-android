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
#include "engine/engines.h"
#include "util/FrameCounter.h"

#include <opencv2/opencv.hpp>
#include <ncnn/mat.h>
#include <ncnn/net.h>
#include "image_process/image_process.h"

namespace test09 {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;

        bool operator==(const Vertex &other) const {
            return position == other.position && uv == other.uv;
        }
    };

    class Test09FaceImageDetection : public test::TestBase {
    private:

        const android_app &mApp;

        // config
        bool mMsaaEnable = false;
        bool mDepthTestEnable = false;
        std::array<float, 4> mClearColor = {0.2f, 0.4f, 0.8f, 1.0f};
        float mClearDepth = 1.0f;
        uint32_t mFrameCount = 2;
        vk::SampleCountFlagBits mSampleCount;

        // vulkan instance
        std::unique_ptr<vklite::Instance> mInstance;
        std::unique_ptr<vklite::Surface> mSurface;
        std::unique_ptr<vklite::PhysicalDevice> mPhysicalDevice;

        std::unique_ptr<vklite::Device> mDevice;

        std::unique_ptr<vklite::Queue> mGraphicQueue;
        std::unique_ptr<vklite::Queue> mPresentQueue;

        std::unique_ptr<vklite::Swapchain> mSwapchain;
        std::vector<vklite::ImageView> mDisplayImageViews;
        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;

        std::unique_ptr<vklite::CommandPool> mCommandPool;
        std::unique_ptr<vklite::CommandBuffers> mCommandBuffers;

        std::unique_ptr<vklite::RenderPass> mRenderPass;
        std::unique_ptr<vklite::CombinedImageView> mColorImageView;
        std::unique_ptr<vklite::CombinedImageView> mDepthImageView;
        vklite::Framebuffers mFramebuffers;

        std::vector<vklite::Semaphore> mImageAvailableSemaphores;
        std::vector<vklite::Semaphore> mRenderFinishedSemaphores;
        std::vector<vklite::Fence> mFences;

        std::unique_ptr<vklite::PipelineLayout> mPipelineLayout;
        std::unique_ptr<vklite::DescriptorPool> mDescriptorPool;
        std::unique_ptr<vklite::DescriptorSetLayouts> mDescriptorSetLayouts;
        std::vector<std::vector<vk::DescriptorSet>> mDescriptorSets;
        std::vector<vklite::PushConstant> mPushConstants;
        std::unique_ptr<vklite::Pipeline> mPipeline;

        //status
        uint32_t mCurrentFrameIndex = 0;
        bool mFramebufferResized = false;

        // vertex buffer
        std::vector<vk::Buffer> mVertexBuffers;
        std::vector<vk::DeviceSize> mVertexBufferOffsets;

        // index buffer
        vk::Buffer mIndexVkBuffer;
        uint32_t mIndexCount;


        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;


        // model
        cv::Mat mImageMat;

        std::vector<vklite::CombinedImageSampler> mSamplers;

        std::vector<image_process::Anchor> mAnchors;
        image_process::LetterBox mLetterBox;

        ncnn::Mat mMatIn;
        ncnn::Net mNet;
        std::unique_ptr<ncnn::Extractor> mExtractor;

        util::FrameCounter mFrameCounter;

    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test09FaceImageDetection(const android_app &app, const std::string &name);

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

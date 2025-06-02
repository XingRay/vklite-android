//
// Created by leixing on 2025/1/4.
//

#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "game_activity/native_app_glue/android_native_app_glue.h"
#include "test/common/glm.h"
#include "test/common/TestBase.h"

#include "vklite/vklite.h"
#include "vklite/Log.h"

namespace test01 {

    struct Vertex {
        glm::vec3 pos;
    };

    class Test01SimpleTriangle : public test::TestBase {
    private:

        const android_app &mApp;

        //config
        const int mFrameCount = 2;
        const std::array<float, 4> mClearColor = {0.2f, 0.4f, 0.6f, 1.0f};
        const float mClearDepth = 1.0f;
        bool mMsaaEnable = true;
        bool mDepthTestEnable = true;

        //status
        uint32_t mCurrentFrameIndex = 0;
        bool mFramebufferResized = false;

        std::unique_ptr<vklite::Instance> mInstance;
        std::unique_ptr<vklite::Surface> mSurface;
        std::unique_ptr<vklite::PhysicalDevice> mPhysicalDevice;

        std::vector<vk::SurfaceFormatKHR> mSurfaceFormats;
        std::vector<vk::PresentModeKHR> mPresentModes;

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
        std::unique_ptr<vklite::CombinedMemoryImage> mColorImage;
        std::unique_ptr<vklite::CombinedImageView> mColorImageView;
        std::unique_ptr<vklite::CombinedMemoryImage> mDepthImage;
        std::unique_ptr<vklite::CombinedImageView> mDepthImageView;
        vklite::Framebuffers mFramebuffers;

        std::vector<vklite::Semaphore> mImageAvailableSemaphores;
        std::vector<vklite::Semaphore> mRenderFinishedSemaphores;
        std::vector<vklite::Fence> mFences;

        std::unique_ptr<vklite::DescriptorPool> mGraphicsDescriptorPool;
        std::unique_ptr<vklite::PipelineLayout> mGraphicsPipelineLayout;
        std::unique_ptr<vklite::Pipeline> mGraphicsPipeline;

        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;

    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test01SimpleTriangle(const android_app &app, const std::string &name);

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
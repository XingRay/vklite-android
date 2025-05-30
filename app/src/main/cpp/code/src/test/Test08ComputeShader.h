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
#include "vklite/vklite_android.h"
#include "util/Timer.h"

namespace test08 {

    struct UniformBufferObject {
        float deltaTime = 1.0f;
    };

    struct Particle {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 color;
    };

    class Test08ComputeShader : public test::TestBase {
    private:

        const android_app &mApp;

        //config
        const int mFrameCount = 2;
        const std::array<float, 4> mClearColor = {0.2f, 0.4f, 0.6f, 1.0f};
        // vertex shader 中设置 z = 1.0, 要启用深度测试, 需要调整 mClearDepth , 满足 depth(默认视角下为 z 值) < mClearDepth 才能正常显示
        const float mClearDepth = 2.0f;
        bool mMsaaEnable = true;
        bool mDepthTestEnable = true;
        static constexpr uint32_t mParticleCount = 8192;


        //status
        uint32_t mCurrentFrameIndex = 0;
        bool mFramebufferResized = false;
        util::Timer mTimer;

        // vk instance
        std::unique_ptr<vklite::Instance> mInstance;
        std::unique_ptr<vklite::Surface> mSurface;
        std::unique_ptr<vklite::PhysicalDevice> mPhysicalDevice;

        std::vector<vk::SurfaceFormatKHR> mSurfaceFormats;
        std::vector<vk::PresentModeKHR> mPresentModes;

        std::unique_ptr<vklite::Device> mDevice;

        std::unique_ptr<vklite::Queue> mGraphicQueue;
        std::unique_ptr<vklite::Queue> mPresentQueue;
        std::unique_ptr<vklite::Queue> mComputeQueue;

        std::unique_ptr<vklite::Swapchain> mSwapchain;
        std::unique_ptr<vklite::CommandPool> mCommandPool;
        std::unique_ptr<vklite::CommandBuffers> mCommandBuffers;
        std::unique_ptr<vklite::RenderPass> mRenderPass;

        std::vector<vklite::ImageView> mDisplayImageViews;

        std::unique_ptr<vklite::CombinedImageView> mColorImageView;
        std::unique_ptr<vklite::CombinedImageView> mDepthImageView;

//        std::vector<vklite::FrameBuffer> mFramebuffers;
        vklite::Framebuffers mFramebuffers;

        std::vector<vklite::Semaphore> mImageAvailableSemaphores;
        std::vector<vklite::Semaphore> mRenderFinishedSemaphores;
        std::vector<vklite::Fence> mFences;

        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;

        std::unique_ptr<vklite::GraphicsPipeline> mGraphicsPipeline;
        std::unique_ptr<vklite::DescriptorPool> mGraphicsDescriptorPool;
        std::unique_ptr<vklite::PipelineLayout> mGraphicsPipelineLayout;
        std::vector<vklite::PipelineResource> mGraphicsPipelineResources;
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;


        std::unique_ptr<vklite::CommandBuffers> mComputeCommandBuffers;
        std::unique_ptr<vklite::ComputePipeline> mComputePipeline;
        std::unique_ptr<vklite::DescriptorPool> mComputeDescriptorPool;
        std::unique_ptr<vklite::PipelineLayout> mComputePipelineLayout;
        std::vector<vklite::PipelineResource> mComputePipelineResources;
        std::vector<vklite::DeviceLocalBuffer> mShaderStorageBuffers;
        std::vector<std::unique_ptr<vklite::BufferInterface>> mUniformBuffers;
        std::vector<vklite::Fence> mComputeFences;
        std::vector<vklite::Semaphore> mComputeFinishSemaphores;

    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test08ComputeShader(const android_app &app, const std::string &name);

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

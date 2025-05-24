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

namespace test08 {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;

        bool operator==(const Vertex &other) const {
            return position == other.position && uv == other.uv;
        }
    };

    struct MvpMatrix {
        alignas(16)glm::mat4 model; // 模型矩阵
        alignas(16)glm::mat4 view;  // 视图矩阵
        alignas(16)glm::mat4 proj;  // 投影矩阵
    };

    class Test08ComputeShader : public test::TestBase {
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
        bool mFrameBufferResized = false;

        std::unique_ptr<vklite::Instance> mInstance;
        std::unique_ptr<vklite::Surface> mSurface;
        std::unique_ptr<vklite::PhysicalDevice> mPhysicalDevice;

        std::vector<vk::SurfaceFormatKHR> mSurfaceFormats;
        std::vector<vk::PresentModeKHR> mPresentModes;

        std::unique_ptr<vklite::Device> mDevice;
        std::unique_ptr<vklite::Swapchain> mSwapchain;
        std::unique_ptr<vklite::CommandPool> mCommandPool;
        std::unique_ptr<vklite::CommandBuffers> mCommandBuffers;
        std::unique_ptr<vklite::RenderPass> mRenderPass;

        std::vector<vklite::ImageView> mDisplayImageViews;

        std::unique_ptr<vklite::ImageInterface> mColorImage;
        std::unique_ptr<vklite::ImageView> mColorImageView;

        std::unique_ptr<vklite::ImageInterface> mDepthImage;
        std::unique_ptr<vklite::ImageView> mDepthImageView;

//        std::vector<vklite::FrameBuffer> mFrameBuffers;
        vklite::FrameBuffers mFrameBuffers;

        std::unique_ptr<vklite::SyncObject> mSyncObject;
        std::unique_ptr<vklite::DescriptorPool> mDescriptorPool;
        std::unique_ptr<vklite::PipelineLayout> mPipelineLayout;

        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;

        std::unique_ptr<vklite::GraphicsPipeline> mGraphicsPipeline;
        std::vector<vklite::PipelineResource> mPipelineResources;
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;

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

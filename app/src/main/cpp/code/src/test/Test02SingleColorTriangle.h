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

namespace test02 {

    struct Vertex {
        glm::vec3 pos;
    };

    struct ColorUniformBufferObject {
        // 颜色数据
        alignas(16) glm::vec3 color;
    };


    class Test02SingleColorTriangle : public test::TestBase {
    private:

        const android_app &mApp;
        const int mFrameCount = 2;
        uint32_t mCurrentFrameIndex = 0;
        bool mFrameBufferResized = false;
        const std::array<float, 4> mClearColor = {0.2f, 0.4f, 0.6f, 1.0f};
        const std::array<float, 4> mDepthStencil = {1.0f, 0, 0, 0};

        std::unique_ptr<vklite::Instance> mInstance;
        std::unique_ptr<vklite::Surface> mSurface;
        std::unique_ptr<vklite::PhysicalDevice> mPhysicalDevice;

        vk::SurfaceCapabilitiesKHR mSurfaceCapabilities;
        std::vector<vk::SurfaceFormatKHR> mSurfaceFormats;
        std::vector<vk::PresentModeKHR> mPresentModes;

        std::unique_ptr<vklite::Device> mDevice;
        std::unique_ptr<vklite::Swapchain> mSwapchain;
        std::unique_ptr<vklite::CommandPool> mCommandPool;
        std::unique_ptr<vklite::RenderPass> mRenderPass;

        std::vector<vklite::ImageView> mDisplayImageViews;

        std::unique_ptr<vklite::ImageInterface> mColorImage;
        std::unique_ptr<vklite::ImageView> mColorImageView;

        std::unique_ptr<vklite::ImageInterface> mDepthImage;
        std::unique_ptr<vklite::ImageView> mDepthImageView;

        std::vector<vklite::FrameBuffer> mFrameBuffers;

        std::unique_ptr<vklite::SyncObject> mSyncObject;
        std::unique_ptr<vklite::GraphicsPipeline> mGraphicsPipeline;
        std::vector<vklite::PipelineResource> mPipelineResources;
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;

    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test02SingleColorTriangle(const android_app &app, const std::string &name);

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

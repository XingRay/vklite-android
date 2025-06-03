//
// Created by leixing on 2025/6/4.
//

#pragma once

#include <memory>

#include "vklite/instance/Instance.h"
#include "vklite/surface/Surface.h"
#include "vklite/physical_device/PhysicalDevice.h"
#include "vklite/device/Device.h"
#include "vklite/device/Queue.h"
#include "vklite/command_pool/CommandPool.h"
#include "vklite/command_buffer/CommandBuffer.h"
#include "vklite/command_buffer/PooledCommandBuffer.h"
#include "vklite/command_buffer/CommandBuffers.h"
#include "vklite/sync/Fence.h"
#include "vklite/sync/Semaphore.h"
#include "vklite/swapchain/Swapchain.h"
#include "vklite/render_pass/RenderPass.h"
#include "vklite/frame_buffer/Framebuffer.h"
#include "vklite/frame_buffer/Framebuffers.h"
#include "vklite/pipeline/shader_module/ShaderModule.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayout.h"
#include "vklite/pipeline/descriptor_pool/DescriptorPool.h"
#include "vklite/pipeline/pipeline/Pipeline.h"
#include "vklite/image/Image.h"
#include "vklite/image/combined_memory_image/CombinedMemoryImage.h"
#include "vklite/image_view/ImageView.h"
#include "vklite/image_view/combined_image_view/CombinedImageView.h"
#include "vklite/sampler/Sampler.h"


#include "vklite/buffer/index_buffer/IndexBuffer.h"
#include "vklite/buffer/index_buffer/IndexBufferBuilder.h"

#include "vklite/buffer/vertex_buffer/VertexBuffer.h"
#include "vklite/buffer/vertex_buffer/VertexBufferBuilder.h"

namespace vklite {

    class SimpleGraphicEngine {
    private:

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

        std::unique_ptr<vklite::DescriptorPool> mGraphicDescriptorPool;
        std::unique_ptr<vklite::PipelineLayout> mGraphicPipelineLayout;
        std::unique_ptr<vklite::Pipeline> mGraphicPipeline;

        // config
        uint32_t mFrameCount;

        //status
        uint32_t mCurrentFrameIndex = 0;
        bool mFramebufferResized = false;

        // vertex buffer
        std::vector<vk::Buffer> mVertexBuffers;
        std::vector<vk::DeviceSize> mVertexBufferOffsets;

        // index buffer
        vk::Buffer mIndexBuffer;
        uint32_t mIndexCount;

    public:
        SimpleGraphicEngine(
                uint32_t frameCount,
                std::unique_ptr<vklite::Instance> instance,
                std::unique_ptr<vklite::Surface> surface,
                std::unique_ptr<vklite::PhysicalDevice> physicalDevice,
                std::unique_ptr<vklite::Device> device,
                std::unique_ptr<vklite::Queue> graphicQueue,
                std::unique_ptr<vklite::Queue> presentQueue,
                std::unique_ptr<vklite::Swapchain> swapchain,
                std::vector<vklite::ImageView> &&displayImageViews,
                std::vector<vk::Viewport> &&viewports,
                std::vector<vk::Rect2D> &&scissors,
                std::unique_ptr<vklite::CommandPool> commandPool,
                std::unique_ptr<vklite::CommandBuffers> commandBuffers,
                std::unique_ptr<vklite::RenderPass> renderPass,
                std::unique_ptr<vklite::CombinedImageView> colorImageView,
                std::unique_ptr<vklite::CombinedImageView> depthImageView,
                vklite::Framebuffers &&framebuffers,
                std::vector<vklite::Semaphore> &&imageAvailableSemaphores,
                std::vector<vklite::Semaphore> &&renderFinishedSemaphores,
                std::vector<vklite::Fence> &&fences,
                std::unique_ptr<vklite::DescriptorPool> graphicDescriptorPool,
                std::unique_ptr<vklite::PipelineLayout> graphicPipelineLayout,
                std::unique_ptr<vklite::Pipeline> graphicPipeline);

        ~SimpleGraphicEngine();

        SimpleGraphicEngine(const SimpleGraphicEngine &other) = delete;

        SimpleGraphicEngine &operator=(const SimpleGraphicEngine &other) = delete;

        SimpleGraphicEngine(SimpleGraphicEngine &&other) noexcept;

        SimpleGraphicEngine &operator=(SimpleGraphicEngine &&other) noexcept;

        [[nodiscard]]
        Device &getDevice() const;

        [[nodiscard]]
        PhysicalDevice &getPhysicalDevice() const;

        [[nodiscard]]
        CommandPool &getCommandPool() const;


        VertexBufferBuilder vertexBufferBuilder();

        SimpleGraphicEngine &addVertexBuffer(const vk::Buffer &buffer, vk::DeviceSize offset = 0);

        SimpleGraphicEngine &addVertexBuffer(const VertexBuffer &buffer, vk::DeviceSize offset = 0);


        IndexBufferBuilder indexBufferBuilder();

        SimpleGraphicEngine &indexBuffer(const vk::Buffer &buffer, uint32_t indexCount);

        SimpleGraphicEngine &indexBuffer(const IndexBuffer &buffer, uint32_t indexCount);

        void drawIndexed();
    };

} // vklite

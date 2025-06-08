//
// Created by leixing on 2025/6/4.
//

#pragma once

#include <memory>
#include <cstdint>

#include "vklite/vklite.h"

namespace vklite {

    class GraphicPipelineEngine {
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

        std::unique_ptr<vklite::PipelineLayout> mPipelineLayout;
        std::unique_ptr<DescriptorPool> mDescriptorPool;
        DescriptorSetLayouts mDescriptorSetLayouts;
        std::vector<std::vector<vk::DescriptorSet>> mDescriptorSets;
        std::vector<PushConstant> mPushConstants;
        std::unique_ptr<vklite::Pipeline> mPipeline;

        // config
        uint32_t mFrameCount;
        vk::SampleCountFlagBits mSampleCount;

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
        GraphicPipelineEngine(
                uint32_t frameCount,
                vk::SampleCountFlagBits sampleCount,
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
                std::unique_ptr<vklite::PipelineLayout> pipelineLayout,
                std::unique_ptr<DescriptorPool> descriptorPool,
                DescriptorSetLayouts &&descriptorSetLayouts,
                std::vector<std::vector<vk::DescriptorSet>> &&descriptorSets,
                std::vector<PushConstant> &&pushConstants,
                std::unique_ptr<vklite::Pipeline> pipeline);

        ~GraphicPipelineEngine();

        GraphicPipelineEngine(const GraphicPipelineEngine &other) = delete;

        GraphicPipelineEngine &operator=(const GraphicPipelineEngine &other) = delete;

        GraphicPipelineEngine(GraphicPipelineEngine &&other) noexcept;

        GraphicPipelineEngine &operator=(GraphicPipelineEngine &&other) noexcept;

        [[nodiscard]]
        Device &getDevice() const;

        [[nodiscard]]
        PhysicalDevice &getPhysicalDevice() const;

        [[nodiscard]]
        uint32_t getFrameCount() const;

        [[nodiscard]]
        uint32_t getFrameIndex() const;

        [[nodiscard]]
        CommandPool &getCommandPool() const;

        [[nodiscard]]
        const std::vector<std::vector<vk::DescriptorSet>> &getDescriptorSets() const;

        [[nodiscard]]
        const vk::DescriptorSet &getDescriptorSets(uint32_t frameIndex, uint32_t set) const;

        GraphicPipelineEngine &updateDescriptorSets(std::function<void(uint32_t, DescriptorSetMappingConfigure &)> &&configure);


        GraphicPipelineEngine &addVertexBuffer(const vk::Buffer &buffer, vk::DeviceSize offset = 0);

        GraphicPipelineEngine &addVertexBuffer(const VertexBuffer &buffer, vk::DeviceSize offset = 0);


        GraphicPipelineEngine &indexBuffer(const vk::Buffer &buffer, uint32_t indexCount);

        GraphicPipelineEngine &indexBuffer(const IndexBuffer &buffer, uint32_t indexCount);


        StorageBufferBuilder storageBufferBuilder();

        StagingBufferBuilder stagingBufferBuilder();

        GraphicPipelineEngine &updatePushConstant(uint32_t index, const void *data, uint32_t size);

        void drawIndexed();


        VertexBufferBuilder vertexBufferBuilder();

        IndexBufferBuilder indexBufferBuilder();

        UniformBufferBuilder uniformBufferBuilder();

        CombinedImageSamplerBuilder samplerBuilder();
    };

} // vklite

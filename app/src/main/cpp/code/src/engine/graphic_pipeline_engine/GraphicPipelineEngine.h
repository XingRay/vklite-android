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

        std::unique_ptr<Instance> mInstance;
        std::unique_ptr<Surface> mSurface;
        std::unique_ptr<PhysicalDevice> mPhysicalDevice;

        std::unique_ptr<Device> mDevice;

        std::unique_ptr<Queue> mGraphicQueue;
        std::unique_ptr<Queue> mPresentQueue;

        std::unique_ptr<Swapchain> mSwapchain;
        std::vector<ImageView> mDisplayImageViews;
        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;

        std::unique_ptr<CommandPool> mCommandPool;
        std::unique_ptr<CommandBuffers> mCommandBuffers;

        std::unique_ptr<RenderPass> mRenderPass;
        std::unique_ptr<CombinedImageView> mColorImageView;
        std::unique_ptr<CombinedImageView> mDepthImageView;
        Framebuffers mFramebuffers;

        std::vector<Semaphore> mImageAvailableSemaphores;
        std::vector<Semaphore> mRenderFinishedSemaphores;
        std::vector<Fence> mFences;

        std::unique_ptr<PipelineLayout> mPipelineLayout;
        std::unique_ptr<DescriptorPool> mDescriptorPool;
        DescriptorSetLayouts mDescriptorSetLayouts;
        std::vector<std::vector<vk::DescriptorSet>> mDescriptorSets;
        std::vector<PushConstant> mPushConstants;
        std::unique_ptr<Pipeline> mPipeline;

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
                std::unique_ptr<Instance> instance,
                std::unique_ptr<Surface> surface,
                std::unique_ptr<PhysicalDevice> physicalDevice,
                std::unique_ptr<Device> device,
                std::unique_ptr<Queue> graphicQueue,
                std::unique_ptr<Queue> presentQueue,
                std::unique_ptr<Swapchain> swapchain,
                std::vector<ImageView> &&displayImageViews,
                std::vector<vk::Viewport> &&viewports,
                std::vector<vk::Rect2D> &&scissors,
                std::unique_ptr<CommandPool> commandPool,
                std::unique_ptr<CommandBuffers> commandBuffers,
                std::unique_ptr<RenderPass> renderPass,
                std::unique_ptr<CombinedImageView> colorImageView,
                std::unique_ptr<CombinedImageView> depthImageView,
                Framebuffers &&framebuffers,
                std::vector<Semaphore> &&imageAvailableSemaphores,
                std::vector<Semaphore> &&renderFinishedSemaphores,
                std::vector<Fence> &&fences,
                std::unique_ptr<PipelineLayout> pipelineLayout,
                std::unique_ptr<DescriptorPool> descriptorPool,
                DescriptorSetLayouts &&descriptorSetLayouts,
                std::vector<std::vector<vk::DescriptorSet>> &&descriptorSets,
                std::vector<PushConstant> &&pushConstants,
                std::unique_ptr<Pipeline> pipeline);

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

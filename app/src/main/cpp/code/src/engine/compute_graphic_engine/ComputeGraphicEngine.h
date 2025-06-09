//
// Created by leixing on 2025/6/9.
//

#pragma once

#include <memory>
#include <cstdint>

#include "vklite/vklite.h"

namespace vklite {

    class ComputeGraphicEngine {
    private:

        std::unique_ptr<Instance> mInstance;
        std::unique_ptr<Surface> mSurface;
        std::unique_ptr<PhysicalDevice> mPhysicalDevice;

        std::unique_ptr<Device> mDevice;
        std::unique_ptr<CommandPool> mCommandPool;


        std::unique_ptr<Queue> mGraphicQueue;
        std::unique_ptr<Queue> mPresentQueue;

        std::unique_ptr<Swapchain> mSwapchain;
        std::vector<ImageView> mDisplayImageViews;
        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;


        std::unique_ptr<CommandBuffers> mGraphicCommandBuffers;

        std::unique_ptr<RenderPass> mRenderPass;
        std::unique_ptr<CombinedImageView> mColorImageView;
        std::unique_ptr<CombinedImageView> mDepthImageView;
        Framebuffers mFramebuffers;

        std::vector<Semaphore> mGraphicImageAvailableSemaphores;
        std::vector<Semaphore> mGraphicRenderFinishedSemaphores;
        std::vector<Fence> mGraphicFences;

        std::unique_ptr<PipelineLayout> mGraphicPipelineLayout;
        std::unique_ptr<DescriptorPool> mGraphicDescriptorPool;
        DescriptorSetLayouts mGraphicDescriptorSetLayouts;
        std::vector<std::vector<vk::DescriptorSet>> mGraphicDescriptorSets;
        std::vector<PushConstant> mGraphicPushConstants;
        std::unique_ptr<Pipeline> mGraphicPipeline;

        std::unique_ptr<Queue> mComputeQueue;
        std::unique_ptr<CommandBuffers> mComputeCommandBuffers;
        std::vector<Fence> mComputeFences;
        std::vector<Semaphore> mComputeFinishSemaphores;

        std::unique_ptr<PipelineLayout> mComputePipelineLayout;
        std::unique_ptr<DescriptorPool> mComputeDescriptorPool;
        DescriptorSetLayouts mComputeDescriptorSetLayouts;
        std::vector<std::vector<vk::DescriptorSet>> mComputeDescriptorSets;
        std::vector<PushConstant> mComputePushConstants;
        std::unique_ptr<Pipeline> mComputePipeline;

        // config
        uint32_t mFrameCount;
        vk::SampleCountFlagBits mSampleCount;

        //status
        uint32_t mCurrentFrameIndex = 0;
        bool mFramebufferResized = false;

        // vertex buffer
        std::vector<std::vector<vk::Buffer>> mVertexBuffers;
        std::vector<std::vector<vk::DeviceSize>> mVertexBufferOffsets;

        // index buffer
        vk::Buffer mIndexBuffer;
        uint32_t mIndexCount;

    public:
        explicit ComputeGraphicEngine(
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
                std::unique_ptr<Pipeline> pipeline,

                std::unique_ptr<Queue> computeQueue,
                std::unique_ptr<CommandBuffers> computeCommandBuffers,
                std::vector<Fence> &&computeFences,
                std::vector<Semaphore> &&computeFinishSemaphores,

                std::unique_ptr<PipelineLayout> computePipelineLayout,
                std::unique_ptr<DescriptorPool> computeDescriptorPool,
                DescriptorSetLayouts &&computeDescriptorSetLayouts,
                std::vector<std::vector<vk::DescriptorSet>> &&computeDescriptorSets,
                std::vector<PushConstant> &&computePushConstants,
                std::unique_ptr<Pipeline> computePipeline);

        ~ComputeGraphicEngine();

        ComputeGraphicEngine(const ComputeGraphicEngine &other) = delete;

        ComputeGraphicEngine &operator=(const ComputeGraphicEngine &other) = delete;

        ComputeGraphicEngine(ComputeGraphicEngine &&other) noexcept;

        ComputeGraphicEngine &operator=(ComputeGraphicEngine &&other) noexcept;

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

        [[nodiscard]]
        const std::vector<std::vector<vk::DescriptorSet>> &getComputeDescriptorSets() const ;

        [[nodiscard]]
        const vk::DescriptorSet &getComputeDescriptorSets(uint32_t frameIndex, uint32_t set) const ;

        ComputeGraphicEngine &updateDescriptorSets(std::function<void(uint32_t frameIndex, DescriptorSetMappingConfigure &configure)> &&configure);

        ComputeGraphicEngine &addVertexBuffers(const std::function<void(uint32_t frameIndex, std::vector<vk::Buffer>& buffers, std::vector<vk::DeviceSize>& offsets)>& configure);

        ComputeGraphicEngine &indexBuffer(const vk::Buffer &buffer, uint32_t indexCount);

        ComputeGraphicEngine &indexBuffer(const IndexBuffer &buffer, uint32_t indexCount);


        StorageBufferBuilder storageBufferBuilder();

        StagingBufferBuilder stagingBufferBuilder();

        ComputeGraphicEngine &updatePushConstant(uint32_t index, const void *data, uint32_t size);

        void drawIndexed(uint32_t particleCount);


        VertexBufferBuilder vertexBufferBuilder();

        IndexBufferBuilder indexBufferBuilder();

        UniformBufferBuilder uniformBufferBuilder();

        CombinedImageSamplerBuilder samplerBuilder();
    };

} // vklite

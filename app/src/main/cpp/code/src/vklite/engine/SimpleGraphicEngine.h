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
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetMappingConfigure.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriterBuilder.h"
#include "vklite/image/Image.h"
#include "vklite/image/combined_memory_image/CombinedMemoryImage.h"
#include "vklite/image_view/ImageView.h"
#include "vklite/image_view/combined_image_view/CombinedImageView.h"
#include "vklite/sampler/Sampler.h"


#include "vklite/buffer/device_local/DeviceLocalBuffer.h"
#include "vklite/buffer/device_local/DeviceLocalBufferBuilder.h"

#include "vklite/buffer/host_visible/HostVisibleBuffer.h"
#include "vklite/buffer/host_visible/HostVisibleBufferBuilder.h"

#include "vklite/buffer/staging_buffer/StagingBuffer.h"
#include "vklite/buffer/staging_buffer/StagingBufferBuilder.h"

#include "vklite/buffer/index_buffer/IndexBuffer.h"
#include "vklite/buffer/index_buffer/IndexBufferBuilder.h"

#include "vklite/buffer/vertex_buffer/VertexBuffer.h"
#include "vklite/buffer/vertex_buffer/VertexBufferBuilder.h"

#include "vklite/buffer/uniform_buffer/UniformBuffer.h"
#include "vklite/buffer/uniform_buffer/UniformBufferBuilder.h"

#include "vklite/buffer/storage_buffer/StorageBuffer.h"
#include "vklite/buffer/storage_buffer/StorageBufferBuilder.h"

#include "vklite/push_constant/PushConstant.h"
#include "vklite/pipeline/descriptor_set_layout/DescriptorSetLayouts.h"

#include "vklite/sampler/combined_sampler/CombinedImageSamplerBuilder.h"

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
        SimpleGraphicEngine(
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
        uint32_t getFrameCount() const;

        [[nodiscard]]
        CommandPool &getCommandPool() const;

        [[nodiscard]]
        const std::vector<std::vector<vk::DescriptorSet>> &getDescriptorSets() const;

        [[nodiscard]]
        const vk::DescriptorSet &getDescriptorSets(uint32_t frameIndex, uint32_t set) const;

        SimpleGraphicEngine &updateDescriptorSets(std::function<void(uint32_t, DescriptorSetMappingConfigure &)> &&configure);


        SimpleGraphicEngine &addVertexBuffer(const vk::Buffer &buffer, vk::DeviceSize offset = 0);

        SimpleGraphicEngine &addVertexBuffer(const VertexBuffer &buffer, vk::DeviceSize offset = 0);


        SimpleGraphicEngine &indexBuffer(const vk::Buffer &buffer, uint32_t indexCount);

        SimpleGraphicEngine &indexBuffer(const IndexBuffer &buffer, uint32_t indexCount);


        StorageBufferBuilder storageBufferBuilder();

        StagingBufferBuilder stagingBufferBuilder();

        SimpleGraphicEngine &updatePushConstant(uint32_t index, const void *data, uint32_t size);

        void drawIndexed();


        VertexBufferBuilder vertexBufferBuilder();

        IndexBufferBuilder indexBufferBuilder();

        UniformBufferBuilder uniformBufferBuilder();

        CombinedImageSamplerBuilder samplerBuilder();
    };

} // vklite

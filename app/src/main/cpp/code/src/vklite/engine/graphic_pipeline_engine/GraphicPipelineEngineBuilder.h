//
// Created by leixing on 2025/6/4.
//

#pragma once

#include <memory>
#include <functional>

#include "GraphicPipelineEngine.h"

// plugin
#include "vklite/plugin/PluginInterface.h"
#include "vklite/plugin/combined/CombinedPlugin.h"
#include "vklite/plugin/debug/DebugPlugin.h"
#include "vklite/plugin/surface/SurfacePlugin.h"

// instance
#include "vklite/instance/Instance.h"
#include "vklite/instance/InstanceBuilder.h"

// surface
#include "vklite/surface/Surface.h"
#include "vklite/surface/SurfaceBuilder.h"

// physical device
#include "vklite/physical_device/PhysicalDevice.h"
#include "vklite/physical_device/PhysicalDeviceSelector.h"
#include "vklite/physical_device/msaa/MaxMsaaSampleCountSelector.h"

// device
#include "vklite/device/Device.h"
#include "vklite/device/DeviceBuilder.h"
#include "vklite/device/Queue.h"

// msaa selector
#include "vklite/physical_device/msaa/MsaaSampleCountSelector.h"
#include "vklite/physical_device/msaa/MaxMsaaSampleCountSelector.h"

// command pool
#include "vklite/command_pool/CommandPool.h"
#include "vklite/command_pool/CommandPoolBuilder.h"

// command buffer
#include "vklite/command_buffer/CommandBufferInterface.h"
#include "vklite/command_buffer/CommandBuffer.h"
#include "vklite/command_buffer/PooledCommandBuffer.h"
#include "vklite/command_buffer/CommandBuffers.h"

// sync objects
#include "vklite/sync/Fence.h"
#include "vklite/sync/FenceBuilder.h"
#include "vklite/sync/Semaphore.h"
#include "vklite/sync/SemaphoreBuilder.h"

// swapchain
#include "vklite/swapchain/Swapchain.h"
#include "vklite/swapchain/SwapchainBuilder.h"

// render pass
#include "vklite/render_pass/RenderPass.h"
#include "vklite/render_pass/RenderPassBuilder.h"

// frame buffer
#include "vklite/frame_buffer/Framebuffer.h"
#include "vklite/frame_buffer/FramebufferBuilder.h"
#include "vklite/frame_buffer/Framebuffers.h"
#include "vklite/frame_buffer/FramebuffersBuilder.h"

// shader module
#include "vklite/pipeline/shader_module/ShaderModule.h"
#include "vklite/pipeline/shader_module/ShaderModuleBuilder.h"

// pipeline layout
#include "vklite/pipeline/pipeline_layout/PipelineLayout.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayoutBuilder.h"

// descriptor
#include "vklite/pipeline/descriptor_pool/DescriptorPool.h"
#include "vklite/pipeline/descriptor_pool/DescriptorPoolBuilder.h"
#include "vklite/pipeline/descriptor/DescriptorConfigure.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriterBuilder.h"

// pipeline resource
#include "vklite/pipeline/pipeline_resource/PipelineResource.h"
#include "vklite/pipeline/pipeline_resource/PipelineResourceBuilder.h"
#include "vklite/pipeline/pipeline_resource/PipelineResourcesBuilder.h"

// pipeline
#include "vklite/pipeline/shader/ShaderConfigure.h"
#include "vklite/pipeline/pipeline/Pipeline.h"
#include "vklite/pipeline/pipeline/GraphicsPipelineBuilder.h"
#include "vklite/pipeline/pipeline/ComputePipelineBuilder.h"

// device memory
#include "vklite/device_memory/DeviceMemory.h"
#include "vklite/device_memory/DeviceMemoryBuilder.h"


// buffer
#include "vklite/buffer/Buffer.h"
#include "vklite/buffer/BufferBuilder.h"
#include "vklite/buffer/BufferMeta.h"

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBuffer.h"
#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBufferBuilder.h"

#include "vklite/buffer/device_local/DeviceLocalBuffer.h"
#include "vklite/buffer/device_local/DeviceLocalBufferBuilder.h"

#include "vklite/buffer/host_visible/HostVisibleBuffer.h"
#include "vklite/buffer/host_visible/HostVisibleBufferBuilder.h"

#include "vklite/buffer/index_buffer/IndexBuffer.h"
#include "vklite/buffer/index_buffer/IndexBufferBuilder.h"
#include "vklite/buffer/index_buffer/IndexBufferMeta.h"

#include "vklite/buffer/staging_buffer/StagingBuffer.h"
#include "vklite/buffer/staging_buffer/StagingBufferBuilder.h"

#include "vklite/buffer/storage_buffer/StorageBuffer.h"
#include "vklite/buffer/storage_buffer/StorageBufferBuilder.h"

#include "vklite/buffer/uniform_buffer/UniformBuffer.h"
#include "vklite/buffer/uniform_buffer/UniformBufferBuilder.h"

#include "vklite/buffer/vertex_buffer/VertexBuffer.h"
#include "vklite/buffer/vertex_buffer/VertexBufferBuilder.h"


// image
#include "vklite/image/Image.h"
#include "vklite/image/ImageBuilder.h"
#include "vklite/image/combined_memory_image/CombinedMemoryImage.h"
#include "vklite/image/combined_memory_image/CombinedMemoryImageBuilder.h"

// image view
#include "vklite/image_view/ImageView.h"
#include "vklite/image_view/ImageViewBuilder.h"
#include "vklite/image_view/combined_image_view/CombinedImageView.h"
#include "vklite/image_view/combined_image_view/CombinedImageViewBuilder.h"


// sampler
#include "vklite/sampler/Sampler.h"
#include "vklite/sampler/SamplerBuilder.h"
#include "vklite/image/ImageTransition.h"


// util
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    class GraphicPipelineEngineBuilder {
    private:
        uint32_t mFrameCount = 2;
        std::array<float, 4> mClearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        float mClearDepth = 1.0f;
        bool mMsaaEnable = false;
        bool mDepthTestEnable = false;
        ShaderConfigure mGraphicShaderConfigure;

        InstanceBuilder mInstanceBuilder;
        std::function<std::unique_ptr<Surface>(const Instance &Instance)> mSurfaceBuilder;
        std::function<std::unique_ptr<PhysicalDevice>(const Instance &Instance, const Surface &surface)> mPhysicalDeviceSelector;
        std::function<vk::SampleCountFlagBits(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits)> mSampleCountSelector;
        DeviceBuilder mDeviceBuilder;

    public:
        GraphicPipelineEngineBuilder();

        ~GraphicPipelineEngineBuilder();

        GraphicPipelineEngineBuilder(const GraphicPipelineEngineBuilder &other) = delete;

        GraphicPipelineEngineBuilder &operator=(GraphicPipelineEngineBuilder &other) = delete;

        GraphicPipelineEngineBuilder(GraphicPipelineEngineBuilder &&other) noexcept;

        GraphicPipelineEngineBuilder &operator=(GraphicPipelineEngineBuilder &&other) noexcept;

        GraphicPipelineEngineBuilder &frameCount(uint32_t frameCount);

        GraphicPipelineEngineBuilder &clearColor(const std::array<float, 4> &clearColor);

        GraphicPipelineEngineBuilder &clearColor(float r, float g, float b, float a);

        GraphicPipelineEngineBuilder &clearColor(float r, float g, float b);

        GraphicPipelineEngineBuilder &clearDepth(float clearDepth);

        GraphicPipelineEngineBuilder &addInstancePlugin(std::unique_ptr<PluginInterface> plugin);

        GraphicPipelineEngineBuilder &addDevicePlugin(std::unique_ptr<PluginInterface> plugin);

        GraphicPipelineEngineBuilder &surfaceBuilder(std::function<std::unique_ptr<Surface>(const Instance &Instance)> &&surfaceBuilder);

        GraphicPipelineEngineBuilder &physicalDeviceSelector(std::function<std::unique_ptr<PhysicalDevice>(const Instance &Instance, const Surface &surface)> &&physicalDeviceSelector);

        GraphicPipelineEngineBuilder &sampleCountSelector(std::function<vk::SampleCountFlagBits(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits)> &&sampleCountSelector);

        GraphicPipelineEngineBuilder &enableDepthTest();

        GraphicPipelineEngineBuilder &shaderConfigure(ShaderConfigure &&shaderConfigure);

        [[nodiscard]]
        GraphicPipelineEngine build();

        [[nodiscard]]
        std::unique_ptr<GraphicPipelineEngine> buildUnique();


        /**
         * preset
         */
        GraphicPipelineEngineBuilder &asDefault();
    };

} // vklite

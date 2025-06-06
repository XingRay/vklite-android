//
// Created by leixing on 2025/5/1.
//

#pragma once

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
#include "vklite/image/ImageTransition.h"

// image view
#include "vklite/image_view/ImageView.h"
#include "vklite/image_view/ImageViewBuilder.h"
#include "vklite/image_view/combined_image_view/CombinedImageView.h"
#include "vklite/image_view/combined_image_view/CombinedImageViewBuilder.h"


// sampler
#include "vklite/sampler/Sampler.h"
#include "vklite/sampler/SamplerBuilder.h"
#include "vklite/sampler/combined_sampler/CombinedImageSampler.h"
#include "vklite/sampler/combined_sampler/CombinedImageSamplerBuilder.h"


// util
#include "vklite/util/VulkanUtil.h"

// engines
#include "vklite/engine/SimpleGraphicEngine.h"
#include "vklite/engine/SimpleGraphicEngineBuilder.h"
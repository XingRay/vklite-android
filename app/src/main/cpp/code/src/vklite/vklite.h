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
#include "vklite/sync/fence/Fence.h"
#include "vklite/sync/fence/FenceBuilder.h"

#include "vklite/sync/semaphore/Semaphore.h"
#include "vklite/sync/semaphore/SemaphoreBuilder.h"

#include "vklite/sync/pipeline_barrier/PipelineBarrier.h"
#include "vklite/sync/pipeline_barrier/PipelineBarrierBuilder.h"

#include "vklite/sync/pipeline_barrier/memory_barrier/MemoryBarrierBuilder.h"
#include "vklite/sync/pipeline_barrier/buffer_memory_barrier/BufferMemoryBarrierBuilder.h"
#include "vklite/sync/pipeline_barrier/image_memory_barrier/ImageMemoryBarrierBuilder.h"


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
#include "vklite/pipeline/combined_pipeline/CombinedPipeline.h"
#include "vklite/pipeline/combined_pipeline/CombinedGraphicPipelineBuilder.h"
#include "vklite/pipeline/combined_pipeline/CombinedComputePipelineBuilder.h"

// descriptor set layout
#include "vklite/pipeline/descriptor_set_layout/DescriptorSetLayouts.h"
#include "vklite/pipeline/descriptor_set_layout/DescriptorSetLayoutsBuilder.h"


// descriptor
#include "vklite/pipeline/descriptor_pool/DescriptorPool.h"
#include "vklite/pipeline/descriptor_pool/DescriptorPoolBuilder.h"

// descriptor set writer
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriter.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriters.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWritersBuilder.h"


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
#include "vklite/sampler/combined_sampler/CombinedImageSampler.h"
#include "vklite/sampler/combined_sampler/CombinedImageSamplerBuilder.h"


// util
#include "vklite/util/VulkanUtil.h"
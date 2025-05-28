//
// Created by leixing on 2025/5/1.
//

#pragma once

// instance
#include "vklite/instance/Instance.h"
#include "vklite/instance/InstanceBuilder.h"

// physical device
#include "vklite/physical_device/PhysicalDeviceSelector.h"
#include "vklite/physical_device/msaa/MaxMsaaSampleCountSelector.h"

// device
#include "vklite/device/Device.h"
#include "vklite/device/DeviceBuilder.h"
#include "vklite/device/Queue.h"

// msaa selector
#include "vklite/physical_device/msaa/MsaaSampleCountSelector.h"
#include "vklite/physical_device/msaa/MaxMsaaSampleCountSelector.h"

// command
#include "vklite/command_buffer/CommandPool.h"
#include "vklite/command_buffer/CommandPoolBuilder.h"

// sync objects
#include "vklite/sync/Fence.h"
#include "vklite/sync/FenceBuilder.h"
#include "vklite/sync/Semaphore.h"
#include "vklite/sync/SemaphoreBuilder.h"
#include "vklite/sync/SyncObject.h"
#include "vklite/sync/SyncObjectBuilder.h"

// swapchain
#include "vklite/swapchain/Swapchain.h"
#include "vklite/swapchain/SwapchainBuilder.h"

// render pass
#include "vklite/render_pass/RenderPass.h"
#include "vklite/render_pass/RenderPassBuilder.h"

// frame buffer
#include "vklite/frame_buffer/FrameBufferBuilder.h"
#include "vklite/frame_buffer/FrameBuffersBuilder.h"


// pipeline layout
#include "vklite/pipeline/descriptor_pool/DescriptorPoolBuilder.h"
#include "vklite/pipeline/descriptor/DescriptorConfigure.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriterBuilder.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayout.h"

// pipeline resource
#include "vklite/pipeline/pipeline_resource/PipelineResource.h"
#include "vklite/pipeline/pipeline_resource/PipelineResourceBuilder.h"
#include "vklite/pipeline/pipeline_resource/PipelineResourcesBuilder.h"

// graphic pipeline
#include "vklite/pipeline/graphics_pipeline/GraphicsPipeline.h"
#include "vklite/pipeline/graphics_pipeline/GraphicsPipelineBuilder.h"

//compute pipeline
#include "vklite/pipeline/compute_pipeline/ComputePipeline.h"
#include "vklite/pipeline/compute_pipeline/ComputePipelineBuilder.h"

// vertex buffer
#include "vklite/vertex_buffer/VertexBuffer.h"
#include "vklite/vertex_buffer/VertexBufferBuilder.h"

// index buffer
#include "vklite/index_buffer/IndexBuffer.h"
#include "vklite/index_buffer/IndexBufferBuilder.h"

// image
#include "vklite/image/Image.h"
#include "vklite/image/ImageBuilder.h"

// image view
#include "vklite/image_view/ImageView.h"
#include "vklite/image_view/ImageViewBuilder.h"

// sampler
#include "vklite/sampler/Sampler.h"
#include "vklite/sampler/SamplerBuilder.h"
#include "vklite/image/ImageTransition.h"

// uniform buffer
#include "vklite/uniform_buffer/UniformBuffer.h"
#include "vklite/uniform_buffer/UniformBufferBuilder.h"

// util
#include "vklite/util/VulkanUtil.h"
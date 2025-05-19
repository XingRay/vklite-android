//
// Created by leixing on 2025/5/1.
//

#pragma once

#include "vklite/instance/InstanceBuilder.h"
#include "vklite/physical_device/PhysicalDeviceSelector.h"
#include "vklite/physical_device/msaa/MaxMsaaSampleCountSelector.h"
#include "vklite/device/DeviceBuilder.h"

#include "vklite/command_buffer/CommandPoolBuilder.h"
#include "vklite/sync/SyncObjectBuilder.h"

#include "vklite/swapchain/SwapchainBuilder.h"
#include "vklite/render_pass/RenderPassBuilder.h"
#include "vklite/frame_buffer/FrameBufferBuilder.h"

#include "vklite/pipeline/graphics_pipeline/GraphicsPipelineBuilder.h"
#include "vklite/pipeline/compute_pipeline/ComputePipelineBuilder.h"
#include "vklite/pipeline/descriptor_pool/DescriptorPoolBuilder.h"
#include "vklite/pipeline/descriptor/DescriptorConfigure.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriterBuilder.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayout.h"
#include "vklite/pipeline/pipeline_resource/PipelineResourceBuilder.h"
#include "vklite/pipeline/pipeline_resource/PipelineResourcesBuilder.h"

#include "vklite/index_buffer/IndexBufferBuilder.h"
#include "vklite/vertex_buffer/VertexBufferBuilder.h"

#include "vklite/image_view/ImageViewBuilder.h"
#include "vklite/image/ImageBuilder.h"
#include "vklite/sampler/SamplerBuilder.h"
#include "vklite/image/color_image/ColorImageBuilder.h"


#include "vklite/uniform_buffer/UniformBufferBuilder.h"
#include "vklite/util/VulkanUtil.h"
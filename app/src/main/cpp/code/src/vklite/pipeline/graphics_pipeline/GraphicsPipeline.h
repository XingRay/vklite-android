//
// Created by leixing on 2024/12/28.
//

#pragma once

#include <vector>
#include <unordered_map>

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/swapchain/Swapchain.h"
#include "vklite/render_pass/RenderPass.h"
#include "vklite/shader/VulkanShaderModule.h"
#include "vklite/pipeline/descriptor_pool/DescriptorPool.h"
#include "vklite/command_buffer/CommandPool.h"
#include "vklite/index_buffer/IndexBuffer.h"
#include "vklite/buffer/host_visible/HostVisibleIndexBuffer.h"
#include "vklite/buffer/device_local/DeviceLocalUniformBuffer.h"
#include "vklite/pipeline/descriptor/old/DescriptorBindingSets.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayout.h"
#include "vklite/pipeline/pipeline_resource/PipelineResource.h"

namespace vklite {

    class GraphicsPipeline {
    private:
        const Device &mDevice;
        vk::Pipeline mPipeline;

    public:

        GraphicsPipeline(const Device &device,
                         const RenderPass &renderPass,
                         const VulkanShaderModule &vertexShaderModule,
                         const VulkanShaderModule &fragmentShaderModule,
                         const std::vector<vk::VertexInputBindingDescription> &vertexInputBindingDescriptions,
                         const std::vector<vk::VertexInputAttributeDescription> &vertexInputAttributeDescriptions,
                         const PipelineLayout &pipelineLayout,
                         const std::vector<vk::Viewport> &viewports,
                         const std::vector<vk::Rect2D> &scissors,
                         bool sampleShadingEnable,
                         vk::SampleCountFlagBits sampleCount,
                         bool depthTestEnable);

        ~GraphicsPipeline();

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

        void drawFrame(const vk::CommandBuffer &commandBuffer,
                       const PipelineLayout &pipelineLayout,
                       const PipelineResource &pipelineResource,
                       const std::vector<vk::Viewport> &viewports,
                       const std::vector<vk::Rect2D> &scissors);
    };

} // vklite

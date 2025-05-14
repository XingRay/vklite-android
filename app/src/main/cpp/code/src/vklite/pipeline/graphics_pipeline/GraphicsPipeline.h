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
#include "vklite/command/CommandPool.h"
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
                         bool msaaEnable,
                         vk::SampleCountFlagBits sampleCount,
                         bool depthTestEnable);

        ~GraphicsPipeline();

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

//        [[nodiscard]]
//        const vk::PipelineLayout &getPipelineLayout() const;

//        [[nodiscard]]
//        const std::vector<vk::DescriptorSet> &getDescriptorSets(uint32_t frameIndex) const;
//
//        [[nodiscard]]
//        const std::vector<vk::PushConstantRange> &getPushConstantRanges() const;
//
//        [[nodiscard]]
//        const std::vector<std::vector<uint8_t>> &getPushConstantDataList() const;

//        GraphicsPipeline &createVertexBuffer(size_t size);
//
//        GraphicsPipeline &createVertexBuffer(uint32_t binding, size_t size);
//
//        GraphicsPipeline &updateVertexBuffer(const CommandPool &commandPool, const void *data, size_t size);
//
//        GraphicsPipeline &updateVertexBuffer(const CommandPool &commandPool, uint32_t index, const void *data, size_t size);
//
//        template<typename T>
//        GraphicsPipeline &updateVertexBuffer(const CommandPool &commandPool, const std::vector<T> &data) {
//            return updateVertexBuffer(commandPool, 0, data);
//        }

//        template<typename T>
//        GraphicsPipeline &updateVertexBuffer(const CommandPool &commandPool, uint32_t index, const std::vector<T> &data) {
//            if (index >= mVertexBuffers.size()) {
//                LOG_E("index out of range, index:%d, size:%zu", index, mVertexBuffers.size());
//
//                // Format the error message using std::to_string
//                std::string errorMessage = "updateVertexBuffer: index out of range, index:" +
//                                           std::to_string(index) +
//                                           ", size:" +
//                                           std::to_string(mVertexBuffers.size());
//                throw std::runtime_error(errorMessage);
//            }
//            mVertexBuffers[index]->update(commandPool, data.data(), data.size() * sizeof(T));
//
//            return *this;
//        }

//        GraphicsPipeline &createIndexBuffer(size_t size);
//
//        GraphicsPipeline &updateIndexBuffer(const CommandPool &commandPool, const std::vector<uint32_t> &indices);

//        GraphicsPipeline &updateUniformBuffer(const CommandPool &commandPool, uint32_t frameIndex, uint32_t set, uint32_t binding, const void *data, uint32_t size);

//        GraphicsPipeline &setDescriptorBindingBufferInfo(uint32_t frameIndex, uint32_t set, uint32_t binding, std::unique_ptr<BufferInfo> &&vulkanBufferInfo);
//
//        GraphicsPipeline &setDescriptorBindingImageInfo(uint32_t frameIndex, uint32_t set, uint32_t binding, std::unique_ptr<ImageInfo> &&vulkanImageInfo);

//        GraphicsPipeline &updatePushConstant(uint32_t index, const void *data);

        void drawFrame(const vk::CommandBuffer &commandBuffer,
                       const PipelineLayout &pipelineLayout,
                       const PipelineResource &pipelineResource,
                       const std::vector<vk::Viewport> &viewports,
                       const std::vector<vk::Rect2D> &scissors);

    private:

//        void updateDescriptorSets();
    };

} // vklite

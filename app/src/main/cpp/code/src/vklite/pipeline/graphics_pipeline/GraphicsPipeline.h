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
#include "vklite/pipeline/descriptor/DescriptorPool.h"
#include "vklite/command_pool/CommandPool.h"
#include "vklite/index_buffer/IndexBuffer.h"
#include "vklite/buffer/host_visible/VulkanHostVisibleIndexBuffer.h"
#include "vklite/buffer/device_local/VulkanDeviceLocalUniformBuffer.h"
#include "vklite/pipeline/descriptor/VulkanDescriptorBindingSets.h"
#include "vklite/pipeline/resource/BufferInfo.h"
#include "vklite/pipeline/resource/ImageInfo.h"
#include "vklite/pipeline/descriptor/PipelineLayout.h"
#include "vklite/pipeline/resource/PipelineResource.h"

namespace vklite {

    class GraphicsPipeline {
    private:
        const Device &mDevice;

        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;

        vk::Pipeline mPipeline;

        std::unique_ptr<PipelineLayout> mPipelineLayout;

        // frame -> pipeline resource
//        std::vector<PipelineResource> mPipelineResources;

    public:

        GraphicsPipeline(const Device &device,
                         const Swapchain &swapchain,
                         const RenderPass &renderPass,
                         const VulkanShaderModule &vertexShaderModule,
                         const VulkanShaderModule &fragmentShaderModule,
                         const std::vector<vk::VertexInputBindingDescription> &vertexInputBindingDescriptions,
                         const std::vector<vk::VertexInputAttributeDescription> &vertexInputAttributeDescriptions,
                         std::unique_ptr<PipelineLayout> &&pipelineLayout);

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
//        GraphicsPipeline &updateVertexBuffer(const CommandPool &vulkanCommandPool, const void *data, size_t size);
//
//        GraphicsPipeline &updateVertexBuffer(const CommandPool &vulkanCommandPool, uint32_t index, const void *data, size_t size);
//
//        template<typename T>
//        GraphicsPipeline &updateVertexBuffer(const CommandPool &vulkanCommandPool, const std::vector<T> &data) {
//            return updateVertexBuffer(vulkanCommandPool, 0, data);
//        }

//        template<typename T>
//        GraphicsPipeline &updateVertexBuffer(const CommandPool &vulkanCommandPool, uint32_t index, const std::vector<T> &data) {
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
//            mVertexBuffers[index]->update(vulkanCommandPool, data.data(), data.size() * sizeof(T));
//
//            return *this;
//        }

//        GraphicsPipeline &createIndexBuffer(size_t size);
//
//        GraphicsPipeline &updateIndexBuffer(const CommandPool &vulkanCommandPool, const std::vector<uint32_t> &indices);

//        GraphicsPipeline &updateUniformBuffer(const CommandPool &commandPool, uint32_t frameIndex, uint32_t set, uint32_t binding, const void *data, uint32_t size);

//        GraphicsPipeline &setDescriptorBindingBufferInfo(uint32_t frameIndex, uint32_t set, uint32_t binding, std::unique_ptr<BufferInfo> &&vulkanBufferInfo);
//
//        GraphicsPipeline &setDescriptorBindingImageInfo(uint32_t frameIndex, uint32_t set, uint32_t binding, std::unique_ptr<ImageInfo> &&vulkanImageInfo);

//        GraphicsPipeline &updatePushConstant(uint32_t index, const void *data);

        void drawFrame(const vk::CommandBuffer &commandBuffer, const PipelineResource &pipelineResource);

    private:

//        void updateDescriptorSets();
    };

} // vklite

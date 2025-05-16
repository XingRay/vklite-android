//
// Created by leixing on 2025/4/18.
//

#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>

#include "vklite/push_constant/PushConstant.h"

/**
 * resources for a frame used in pipeline
 * resources: uniform buffer, vk::ImageView ...
 */
namespace vklite {

    class PipelineResource {
    private:

        std::vector<vk::Buffer> mVertexBuffers;
        std::vector<vk::DeviceSize> mVertexBufferOffsets;

        vk::Buffer mIndexBuffer;
        vk::DeviceSize mIndexBufferOffset;
        uint32_t mIndicesCount;

        std::vector<vk::DescriptorSet> mDescriptorSets;

    public:

        PipelineResource(std::vector<vk::Buffer> &&vertexBuffers,
                         std::vector<vk::DeviceSize> &&vertexBufferOffsets,
                         vk::Buffer indexBuffer,
                         vk::DeviceSize indexBufferOffset,
                         uint32_t indicesCount,
                         std::vector<vk::DescriptorSet> &&descriptorSets);

        ~PipelineResource();

        [[nodiscard]]
        const std::vector<vk::Buffer> &getVertexBuffers() const;

        [[nodiscard]]
        const std::vector<vk::DeviceSize> &getVertexBufferOffsets() const;

        [[nodiscard]]
        const vk::Buffer &getIndexBuffer() const;

        [[nodiscard]]
        vk::DeviceSize getIndexBufferOffset() const;

        [[nodiscard]]
        uint32_t getIndicesCount() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorSet> &getDescriptorSets() const;

//        PipelineResource &addVertexBuffer(vk::Buffer buffer, vk::DeviceSize offset);
//
//        PipelineResource &indexBuffer(vk::Buffer buffer, vk::DeviceSize offset);
//
//        PipelineResource &indicesCount(uint32_t indicesCount);
//
//        PipelineResource &descriptorSets(std::vector<vk::DescriptorSet> &&descriptorSets);
    };

} // vklite

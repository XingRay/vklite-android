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

        // vertex buffer
        std::vector<vk::Buffer> mVertexBuffers;
        std::vector<vk::DeviceSize> mVertexBufferOffsets;

        // index buffer
        vk::Buffer mIndexBuffer;
        vk::DeviceSize mIndexBufferOffset;
        vk::IndexType mIndexType;
        uint32_t mIndicesCount;

        // push constants
        std::vector<PushConstant> mPushConstants;

        // descriptor set
        std::vector<vk::DescriptorSet> mDescriptorSets;

    public:

        explicit PipelineResource(std::vector<vk::Buffer> &&vertexBuffers,
                                  std::vector<vk::DeviceSize> &&vertexBufferOffsets,
                                  vk::Buffer indexBuffer,
                                  vk::DeviceSize indexBufferOffset,
                                  vk::IndexType indexType,
                                  uint32_t indicesCount,
                                  std::vector<PushConstant> &&pushConstants,
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
        vk::IndexType getIndexType() const;

        [[nodiscard]]
        uint32_t getIndicesCount() const;

        [[nodiscard]]
        const std::vector<PushConstant> &getPushConstants() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorSet> &getDescriptorSets() const;

        PipelineResource &updatePushConstant(uint32_t index, const void *data, uint32_t size);
    };

} // vklite

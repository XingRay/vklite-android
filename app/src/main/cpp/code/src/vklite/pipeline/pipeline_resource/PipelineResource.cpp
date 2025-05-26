//
// Created by leixing on 2025/4/18.
//

#include "PipelineResource.h"
#include "vklite/Log.h"

namespace vklite {

    PipelineResource::PipelineResource(std::vector<vk::Buffer> &&vertexBuffers,
                                       std::vector<vk::DeviceSize> &&vertexBufferOffsets,
                                       vk::Buffer indexBuffer,
                                       vk::DeviceSize indexBufferOffset,
                                       vk::IndexType indexType,
                                       uint32_t indicesCount,
                                       std::vector<PushConstant> &&pushConstants,
                                       std::vector<vk::DescriptorSet> &&descriptorSets)
            : mVertexBuffers(std::move(vertexBuffers)),
              mVertexBufferOffsets(std::move(vertexBufferOffsets)),
              mIndexBuffer(indexBuffer),
              mIndexBufferOffset(indexBufferOffset),
              mIndexType(indexType),
              mIndicesCount(indicesCount),
              mPushConstants(std::move(pushConstants)),
              mDescriptorSets(std::move(descriptorSets)) {}

    PipelineResource::~PipelineResource() = default;

    const std::vector<vk::Buffer> &PipelineResource::getVertexBuffers() const {
        return mVertexBuffers;
    }

    const std::vector<vk::DeviceSize> &PipelineResource::getVertexBufferOffsets() const {
        return mVertexBufferOffsets;
    }

    const vk::Buffer &PipelineResource::getIndexBuffer() const {
        return mIndexBuffer;
    }

    vk::DeviceSize PipelineResource::getIndexBufferOffset() const {
        return mIndexBufferOffset;
    }

    vk::IndexType PipelineResource::getIndexType() const {
        return mIndexType;
    }

    uint32_t PipelineResource::getIndicesCount() const {
        return mIndicesCount;
    }

    const std::vector<PushConstant> &PipelineResource::getPushConstants() const {
        return mPushConstants;
    }

    const std::vector<vk::DescriptorSet> &PipelineResource::getDescriptorSets() const {
        return mDescriptorSets;
    }

    PipelineResource &PipelineResource::updatePushConstant(uint32_t index, const void *data, uint32_t size) {
        mPushConstants[index].update(data, size);
        return *this;
    }

} // vklite
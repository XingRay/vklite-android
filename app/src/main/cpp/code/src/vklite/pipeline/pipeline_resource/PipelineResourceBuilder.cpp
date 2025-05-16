//
// Created by leixing on 2025/4/25.
//

#include "PipelineResourceBuilder.h"

namespace vklite {

    PipelineResourceBuilder::PipelineResourceBuilder()
            : mIndicesCount(0) {};

    PipelineResourceBuilder::~PipelineResourceBuilder() = default;

    PipelineResourceBuilder &PipelineResourceBuilder::addVertexBuffer(const VertexBufferInfo &vertexBufferInfo) {
        mVertexBufferInfos.push_back(vertexBufferInfo);
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::addVertexBuffer(const VertexBuffer &vertexBuffer, vk::DeviceSize offset) {
        mVertexBufferInfos.emplace_back(vertexBuffer, offset);
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::indexBuffer(const IndexBufferInfo &indexBufferInfo) {
        mIndexBufferInfo = std::make_unique<IndexBufferInfo>(indexBufferInfo);
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::indexBuffer(const IndexBuffer &indexBuffer, vk::DeviceSize offset) {
        mIndexBufferInfo = std::make_unique<IndexBufferInfo>(indexBuffer, offset);
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::indicesCount(uint32_t indicesCount) {
        mIndicesCount = indicesCount;
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::descriptorSets(std::vector<vk::DescriptorSet> &&descriptorSets) {
        mDescriptorSets = std::move(descriptorSets);
        return *this;
    }

    PipelineResource PipelineResourceBuilder::build() {
        std::vector<vk::Buffer> vertexBuffers;
        std::vector<vk::DeviceSize> vertexBufferOffsets;
        vertexBuffers.reserve(mVertexBufferInfos.size());
        vertexBufferOffsets.reserve(mVertexBufferInfos.size());
        for (const VertexBufferInfo &vertexBufferInfo: mVertexBufferInfos) {
            vertexBuffers.push_back(vertexBufferInfo.getVertexBuffer().getBuffer());
            vertexBufferOffsets.push_back(vertexBufferInfo.getOffset());
        }

//        return PipelineResource(std::move(vertexBuffers),
//                                std::move(vertexBufferOffsets),
//                                mIndexBufferInfo->getIndexBuffer().getBuffer(),
//                                mIndexBufferInfo->getOffset(),
//                                mIndicesCount,
//                                std::move(mDescriptorSets));
        return {std::move(vertexBuffers),
                std::move(vertexBufferOffsets),
                mIndexBufferInfo->getIndexBuffer().getBuffer(),
                mIndexBufferInfo->getOffset(),
                mIndicesCount,
                std::move(mDescriptorSets)};
    }

} // vklite
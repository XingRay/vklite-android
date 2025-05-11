//
// Created by leixing on 2025/4/25.
//

#include "PipelineResourceBuilder.h"

namespace vklite {

    PipelineResourceBuilder::PipelineResourceBuilder()
            : mIndicesCount(0) {};

    PipelineResourceBuilder::~PipelineResourceBuilder() = default;

    PipelineResourceBuilder &PipelineResourceBuilder::vertexBuffer(std::function<VertexBuffer &(uint32_t frameIndex)> &&vertexBufferProvider) {
        mVertexBufferProvider = std::move(vertexBufferProvider);
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::vertexBuffer(VertexBuffer &vertexBuffer) {
        mVertexBufferProvider = [&vertexBuffer](uint32_t frameIndex) -> VertexBuffer & {
            return vertexBuffer;
        };
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::indexBuffer(std::function<IndexBuffer &(uint32_t frameIndex)> &&indexBufferProvider) {
        mIndexBufferProvider = std::move(indexBufferProvider);
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::indexBuffer(IndexBuffer &indexBuffer) {
        mIndexBufferProvider = [&indexBuffer](uint32_t frameIndex) -> IndexBuffer & {
            return indexBuffer;
        };
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::indicesCount(uint32_t indicesCount) {
        mIndicesCount = indicesCount;
        return *this;
    }

    std::vector<PipelineResource> PipelineResourceBuilder::build(uint32_t frameCount) {
        std::vector<PipelineResource> pipelineResources;
//        pipelineResources.reserve(frameCount);
        pipelineResources.resize(frameCount);
        for (int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
//            pipelineResources.emplace_back(); // 添加新元素
//            PipelineResource& pipelineResource = pipelineResources.back();
            PipelineResource &pipelineResource = pipelineResources[frameIndex];
            pipelineResource.addVertexBuffer(mVertexBufferProvider(frameIndex).getBuffer(), 0);
            pipelineResource.setIndexBuffer(mIndexBufferProvider(frameIndex).getBuffer(), 0);
            pipelineResource.indicesCount(mIndicesCount);
        }

        return pipelineResources;
    }

} // vklite
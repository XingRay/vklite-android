//
// Created by leixing on 2025/4/25.
//

#include "PipelineResourceBuilder.h"

namespace vklite {

    PipelineResourceBuilder::PipelineResourceBuilder()
            : mIndicesCount(0) {};

    PipelineResourceBuilder::~PipelineResourceBuilder() = default;

    PipelineResourceBuilder &PipelineResourceBuilder::addVertexBuffer(std::function<VertexBuffer &(uint32_t frameIndex)> &&vertexBufferProvider) {
        mVertexBufferProviders.push_back(std::move(vertexBufferProvider));
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::addVertexBuffer(VertexBuffer &vertexBuffer) {
        std::function<VertexBuffer &(uint32_t frameIndex)> vertexBufferProvider = [&vertexBuffer](uint32_t frameIndex) -> VertexBuffer & {
            return vertexBuffer;
        };
        addVertexBuffer(std::move(vertexBufferProvider));
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

    PipelineResourceBuilder &PipelineResourceBuilder::descriptorSet(std::function<std::vector<vk::DescriptorSet>(uint32_t frameIndex)> &&descriptorSetProvider) {
        mDescriptorSetProvider = std::move(descriptorSetProvider);
        return *this;
    }

    std::vector<PipelineResource> PipelineResourceBuilder::build(uint32_t frameCount) {
        std::vector<PipelineResource> pipelineResources;
        pipelineResources.reserve(frameCount);

        for (int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
            PipelineResource pipelineResource;

            for (const std::function<VertexBuffer &(uint32_t)> &vertexBufferProvider: mVertexBufferProviders) {
                pipelineResource
                        .addVertexBuffer(vertexBufferProvider(frameIndex).getBuffer(), 0);
            }

            pipelineResource
                    .indexBuffer(mIndexBufferProvider(frameIndex).getBuffer(), 0)
                    .indicesCount(mIndicesCount)
                    .descriptorSets(mDescriptorSetProvider(frameIndex));

            pipelineResources.push_back(std::move(pipelineResource));
        }

        return pipelineResources;
    }

} // vklite
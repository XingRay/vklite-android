//
// Created by leixing on 2025/4/25.
//

#include "PipelineResourceBuilder.h"

namespace vklite {

    PipelineResourceBuilder::PipelineResourceBuilder()
            : mIndicesCount(0) {};

    PipelineResourceBuilder::~PipelineResourceBuilder() = default;

    PipelineResourceBuilder &PipelineResourceBuilder::addVertexBuffer(VertexBuffer &vertexBuffer, vk::DeviceSize offset) {
        addVertexBuffer(VertexBufferInfo(vertexBuffer, offset));
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::addVertexBuffer(std::function<VertexBufferInfo(uint32_t frameIndex)> &&vertexBufferInfoProvider) {
        mVertexBufferInfoProviders.push_back(std::move(vertexBufferInfoProvider));
        return *this;
    }

    PipelineResourceBuilder &PipelineResourceBuilder::addVertexBuffer(const VertexBufferInfo &vertexBufferInfo) {
        std::function<VertexBufferInfo(uint32_t frameIndex)> provider = [=](uint32_t frameIndex) -> VertexBufferInfo {
            return vertexBufferInfo;
        };
        addVertexBuffer(std::move(provider));
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

            for (const std::function<VertexBufferInfo(uint32_t)> &vertexBufferInfoProvider: mVertexBufferInfoProviders) {
                const VertexBufferInfo &vertexBufferInfo = vertexBufferInfoProvider(frameIndex);
                pipelineResource
                        .addVertexBuffer(vertexBufferInfo.getVertexBuffer().getBuffer(), vertexBufferInfo.getOffset());
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
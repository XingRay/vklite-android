//
// Created by leixing on 2025/4/25.
//

#pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include "vklite/pipeline/pipeline_resource/PipelineResource.h"
#include "vklite/vertex_buffer/VertexBuffer.h"
#include "vklite/index_buffer/IndexBuffer.h"
#include "vklite/pipeline/pipeline_resource/VertexBufferInfo.h"

namespace vklite {

    class PipelineResourceBuilder {
    private:
        std::vector<std::function<VertexBufferInfo(uint32_t)>> mVertexBufferInfoProviders;
        std::function<IndexBuffer &(uint32_t)> mIndexBufferProvider;
        uint32_t mIndicesCount;

        std::function<std::vector<vk::DescriptorSet>(uint32_t frameIndex)> mDescriptorSetProvider;

    public:
        PipelineResourceBuilder();

        ~PipelineResourceBuilder();

        PipelineResourceBuilder &addVertexBuffer(VertexBuffer &vertexBuffer, vk::DeviceSize offset = 0);

        PipelineResourceBuilder &addVertexBuffer(std::function<VertexBufferInfo(uint32_t frameIndex)> &&vertexBufferInfoProvider);

        PipelineResourceBuilder &addVertexBuffer(const VertexBufferInfo &vertexBufferInfo);

        PipelineResourceBuilder &indexBuffer(std::function<IndexBuffer &(uint32_t frameIndex)> &&indexBufferProvider);

        PipelineResourceBuilder &indexBuffer(IndexBuffer &indexBuffer);

        PipelineResourceBuilder &indicesCount(uint32_t indicesCount);

        PipelineResourceBuilder &descriptorSet(std::function<std::vector<vk::DescriptorSet>(uint32_t frameIndex)> &&descriptorSetProvider);

        [[nodiscard]]
        std::vector<PipelineResource> build(uint32_t frameCount);
    };

} // vklite

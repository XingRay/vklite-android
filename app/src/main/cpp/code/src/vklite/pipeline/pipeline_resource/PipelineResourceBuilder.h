//
// Created by leixing on 2025/4/25.
//

#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "vklite/pipeline/pipeline_resource/PipelineResource.h"
#include "vklite/vertex_buffer/VertexBuffer.h"
#include "vklite/index_buffer/IndexBuffer.h"
#include "vklite/pipeline/pipeline_resource/VertexBufferInfo.h"
#include "vklite/pipeline/pipeline_resource/IndexBufferInfo.h"
#include "vklite/pipeline/shader/ShaderConfigure.h"

namespace vklite {

    class PipelineResourceBuilder {
    private:

        // index buffer
        std::vector<VertexBufferInfo> mVertexBufferInfos;

        // index buffer
        std::unique_ptr<IndexBufferInfo> mIndexBufferInfo;
        uint32_t mIndicesCount;

        // push constants
        std::vector<PushConstant> mPushConstants;

        // descriptor set
        std::vector<vk::DescriptorSet> mDescriptorSets;

    public:
        PipelineResourceBuilder();

        ~PipelineResourceBuilder();

        PipelineResourceBuilder &addVertexBuffer(const VertexBufferInfo &vertexBufferInfo);

        PipelineResourceBuilder &addVertexBuffer(const VertexBuffer &vertexBuffer, vk::DeviceSize offset = 0);

        PipelineResourceBuilder &indexBuffer(const IndexBufferInfo &indexBufferInfo);

        PipelineResourceBuilder &indexBuffer(const IndexBuffer &indexBuffer, vk::DeviceSize offset = 0);

        PipelineResourceBuilder &indicesCount(uint32_t indicesCount);

        PipelineResourceBuilder &addPushConstant(PushConstant &&pushConstant);

        PipelineResourceBuilder &descriptorSets(std::vector<vk::DescriptorSet> &&descriptorSets);

        [[nodiscard]]
        PipelineResource build();
    };

} // vklite

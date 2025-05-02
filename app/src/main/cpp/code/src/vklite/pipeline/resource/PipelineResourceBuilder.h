//
// Created by leixing on 2025/4/25.
//

#pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include "vklite/pipeline/resource/PipelineResource.h"
#include "vklite/vertex_buffer/VertexBuffer.h"
#include "vklite/index_buffer/IndexBuffer.h"

namespace vklite {

    class PipelineResourceBuilder {
    private:
        std::function<VertexBuffer &(uint32_t)> mVertexBufferProvider;
        std::function<IndexBuffer &(uint32_t)> mIndexBufferProvider;

    public:
        PipelineResourceBuilder();

        ~PipelineResourceBuilder();

        PipelineResourceBuilder &vertexBuffer(std::function<VertexBuffer &(uint32_t frameIndex)> vertexBufferProvider);

        PipelineResourceBuilder &vertexBuffer(VertexBuffer &vertexBuffer);

        PipelineResourceBuilder &indexBuffer(std::function<IndexBuffer &(uint32_t frameIndex)> indexBufferProvider);

        PipelineResourceBuilder &indexBuffer(IndexBuffer &indexBuffer);

        [[nodiscard]]
        std::vector<PipelineResource> build(uint32_t frameCount);
    };

} // vklite

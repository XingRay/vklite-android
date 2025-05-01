//
// Created by leixing on 2025/4/25.
//

#pragma once

#include <cstdint>
#include <vector>

//#include "vklite/vertex/buffer/VertexBufferConfigure.h"
#include "vklite/index_buffer/IndexBufferBuilder.h"
#include "vklite/pipeline/resource/PipelineResource.h"

namespace vklite {

    class PipelineResourceBuilder {
    private:

        /**
         * vertex buffer
         */
//        VertexBufferConfigure mVertexBufferConfigure;
        /**
         * index buffer
         */
        IndexBufferBuilder mIndexConfigure;

    public:
        PipelineResourceBuilder();

        ~PipelineResourceBuilder();

        /**
         * index
         */
        PipelineResourceBuilder &index(const std::function<void(IndexBufferBuilder &)> &configure);

        PipelineResourceBuilder &index(uint32_t capacity);

        PipelineResourceBuilder &index(uint32_t capacity, std::vector<uint32_t> &&indices);

        PipelineResourceBuilder &index(std::vector<uint32_t> &&indices);

        PipelineResourceBuilder &index(const std::shared_ptr<IndexBuffer> &indexBuffer);

        [[nodiscard]]
        std::vector<PipelineResource> build(uint32_t frameCount);
    };

} // vklite

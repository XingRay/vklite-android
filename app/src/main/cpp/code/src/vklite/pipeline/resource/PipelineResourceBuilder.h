//
// Created by leixing on 2025/4/25.
//

#pragma once

#include <cstdint>
#include <vector>

#include "vklite/vertex/buffer/VertexBufferConfigure.h"
#include "vklite/configure/index_buffer/IndexBufferConfigure.h"
#include "vklite/pipeline/resource/PipelineResource.h"

namespace vklite {

    class PipelineResourceBuilder {
    private:

        /**
         * vertex buffer
         */
        VertexBufferConfigure mVertexBufferConfigure;
        /**
         * index buffer
         */
        IndexBufferConfigure mIndexConfigure;

    public:
        PipelineResourceBuilder();

        ~PipelineResourceBuilder();

        /**
         * index
         */
        PipelineResourceBuilder &index(const std::function<void(IndexBufferConfigure &)> &configure);

        PipelineResourceBuilder &index(uint32_t capacity);

        PipelineResourceBuilder &index(uint32_t capacity, std::vector<uint32_t> &&indices);

        PipelineResourceBuilder &index(std::vector<uint32_t> &&indices);

        PipelineResourceBuilder &index(const std::shared_ptr<VulkanDeviceLocalIndexBuffer> &indexBuffer);

        [[nodiscard]]
        std::vector<PipelineResource> createPipelineResources(uint32_t frameCount);
    };

} // vklite

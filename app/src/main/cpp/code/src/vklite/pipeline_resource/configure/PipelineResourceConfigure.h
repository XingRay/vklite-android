//
// Created by leixing on 2025/4/25.
//

#pragma once

#include <cstdint>
#include <vector>

#include "vklite/vertex/buffer/VertexBufferConfigure.h"
#include "vklite/configure/index_buffer/IndexBufferConfigure.h"
#include "vklite/pipeline_resource/PipelineResource.h"

namespace vklite {

    class PipelineResourceConfigure {
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
        PipelineResourceConfigure();

        ~PipelineResourceConfigure();

        /**
         * index
         */
        PipelineResourceConfigure &index(const std::function<void(IndexBufferConfigure &)> &configure);

        PipelineResourceConfigure &index(uint32_t capacity);

        PipelineResourceConfigure &index(uint32_t capacity, std::vector<uint32_t> &&indices);

        PipelineResourceConfigure &index(std::vector<uint32_t> &&indices);

        PipelineResourceConfigure &index(const std::shared_ptr<VulkanDeviceLocalIndexBuffer> &indexBuffer);

        [[nodiscard]]
        std::vector<PipelineResource> createPipelineResources(uint32_t frameCount);
    };

} // vklite

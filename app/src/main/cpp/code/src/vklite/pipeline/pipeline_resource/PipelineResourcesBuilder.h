//
// Created by leixing on 2025/5/16.
//

#pragma once

#include <vector>
#include <cstdint>
#include <functional>

#include "vklite/pipeline/pipeline_resource/PipelineResource.h"

namespace vklite {

    class PipelineResourcesBuilder {
    private:
        uint32_t mFrameCount;
        std::function<PipelineResource(uint32_t frameIndex)> mPipelineResourceBuilder;

    public:

        PipelineResourcesBuilder();

        ~PipelineResourcesBuilder();

        PipelineResourcesBuilder &frameCount(uint32_t frameCount);

        PipelineResourcesBuilder &pipelineResourceBuilder(std::function<PipelineResource(uint32_t frameIndex)> &&pipelineResourceBuilder);

        [[nodiscard]]
        std::vector<PipelineResource> build() const;

    };

} // vklite

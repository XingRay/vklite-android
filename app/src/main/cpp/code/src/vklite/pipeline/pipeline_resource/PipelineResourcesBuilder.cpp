//
// Created by leixing on 2025/5/16.
//

#include "PipelineResourcesBuilder.h"

namespace vklite {

    PipelineResourcesBuilder::PipelineResourcesBuilder()
            : mFrameCount(0) {}

    PipelineResourcesBuilder::~PipelineResourcesBuilder() = default;

    PipelineResourcesBuilder &PipelineResourcesBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    PipelineResourcesBuilder &PipelineResourcesBuilder::pipelineResourceBuilder(std::function<PipelineResource(uint32_t frameIndex)> &&pipelineResourceBuilder) {
        mPipelineResourceBuilder = std::move(pipelineResourceBuilder);
        return *this;
    }

    std::vector<PipelineResource> PipelineResourcesBuilder::build() const {
        std::vector<PipelineResource> pipelineResources;
        pipelineResources.reserve(mFrameCount);
        for (uint32_t frameIndex = 0; frameIndex < mFrameCount; frameIndex++) {
            pipelineResources.push_back(mPipelineResourceBuilder(frameIndex));
        }
        return pipelineResources;
    }

} // vklite
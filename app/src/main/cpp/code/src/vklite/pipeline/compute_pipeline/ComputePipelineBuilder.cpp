//
// Created by leixing on 2025/3/7.
//

#include "ComputePipelineBuilder.h"

namespace vklite {
    ComputePipelineBuilder::ComputePipelineBuilder() {

    }

    ComputePipelineBuilder::~ComputePipelineBuilder() {

    }

    std::unique_ptr<ComputePipeline> ComputePipelineBuilder::build() {
//        return std::make_unique<VulkanComputePipeline>();
        return nullptr;
    }

} // vklite
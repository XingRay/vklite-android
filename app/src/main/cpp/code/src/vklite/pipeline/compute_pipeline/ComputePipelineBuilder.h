//
// Created by leixing on 2025/3/7.
//

#pragma once
#include <memory>

#include "ComputePipeline.h"

namespace vklite {

    class ComputePipelineBuilder {

    public:
        ComputePipelineBuilder();
        ~ComputePipelineBuilder();

        std::unique_ptr<ComputePipeline> build();

    };

} // vklite

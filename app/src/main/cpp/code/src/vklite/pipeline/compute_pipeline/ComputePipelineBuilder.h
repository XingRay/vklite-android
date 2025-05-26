//
// Created by leixing on 2025/3/7.
//

#pragma once

#include <memory>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/compute_pipeline/ComputePipeline.h"

namespace vklite {

    class ComputePipelineBuilder {
    private:
        vk::Device mDevice;
        std::vector<uint32_t> mComputeShaderCode;
        vk::PipelineLayout mPipelineLayout;

    public:
        ComputePipelineBuilder();

        ~ComputePipelineBuilder();

        ComputePipelineBuilder& device(vk::Device device);

        ComputePipelineBuilder& computeShaderCode(std::vector<uint32_t>&& computeShaderCode);

        ComputePipelineBuilder& pipelineLayout(vk::PipelineLayout pipelineLayout);

        std::optional<ComputePipeline> build();

        std::unique_ptr<ComputePipeline> buildUnique();

    };

} // vklite

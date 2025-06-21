//
// Created by leixing on 2025/3/7.
//

#pragma once

#include <memory>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/pipeline/Pipeline.h"
#include "vklite/pipeline/shader_module/ShaderModule.h"

namespace vklite {

    class ComputePipelineBuilder {
    private:
        vk::Device mDevice;

        std::unique_ptr<ShaderModule> mComputeShaderModule;
        vk::PipelineShaderStageCreateInfo mComputeShaderStageInfo;
        vk::ComputePipelineCreateInfo mComputePipelineCreateInfo;
        vk::PipelineCache mPipelineCache;

    public:
        explicit ComputePipelineBuilder();

        ~ComputePipelineBuilder();

        ComputePipelineBuilder &device(vk::Device device);

        ComputePipelineBuilder &computeShader(std::unique_ptr<ShaderModule> &&computeShaderModule);

        ComputePipelineBuilder &pipelineLayout(vk::PipelineLayout pipelineLayout);

        ComputePipelineBuilder &pipelineCache(vk::PipelineCache pipelineCache);

        Pipeline build();

        std::unique_ptr<Pipeline> buildUnique();

    };

} // vklite

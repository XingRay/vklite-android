//
// Created by leixing on 2025-06-19.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vklite/pipeline/shader/ShaderConfigure.h"
#include "vklite/pipeline/combined_pipeline/CombinedPipeline.h"
#include "vklite/pipeline/descriptor_set_layout/DescriptorSetLayoutsBuilder.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayoutBuilder.h"
#include "vklite/pipeline/shader_module/ShaderModuleBuilder.h"
#include "vklite/pipeline/pipeline/ComputePipelineBuilder.h"
#include "vklite/push_constant/PushConstant.h"

namespace vklite {

    class CombinedComputePipelineBuilder {
    private:
        uint32_t mFrameCount;
        vk::Device mDevice;
        vk::DescriptorPool mDescriptorPool;

        std::vector<vk::PushConstantRange> mPushConstantRanges;
        DescriptorSetLayoutsBuilder mDescriptorSetLayoutsBuilder;
        PipelineLayoutBuilder mPipelineLayoutBuilder;

        ShaderModuleBuilder mComputeShaderModuleBuilder;

        ComputePipelineBuilder mComputePipelineBuilder;

    public:
        CombinedComputePipelineBuilder();

        ~CombinedComputePipelineBuilder();

        CombinedComputePipelineBuilder &frameCount(uint32_t frameCount);

        CombinedComputePipelineBuilder &device(vk::Device device);

        CombinedComputePipelineBuilder &descriptorPool(vk::DescriptorPool descriptorPool);


        CombinedComputePipelineBuilder &pushConstantRange(std::vector<vk::PushConstantRange> &&pushConstantRanges);

        CombinedComputePipelineBuilder &bindings(std::vector<std::vector<vk::DescriptorSetLayoutBinding>> &&descriptorSetLayoutBindings);

        CombinedComputePipelineBuilder &computeShaderCode(std::vector<uint32_t> &&code);


        CombinedComputePipelineBuilder &shaderConfigure(ShaderConfigure &shaderConfigure);


        CombinedPipeline build();

        std::unique_ptr<CombinedPipeline> buildUnique();
    };

} // vklite

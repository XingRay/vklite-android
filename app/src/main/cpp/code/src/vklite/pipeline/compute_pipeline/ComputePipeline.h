//
// Created by leixing on 2024/12/28.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/shader/ShaderModule.h"

namespace vklite {

    class ComputePipeline {
    private:
        const Device &mDevice;
        vk::Pipeline mPipeline;
        vk::PipelineLayout mPipelineLayout;

    public:
        ComputePipeline(
                const Device &device,
                const ShaderModule &computeShaderModule,
                const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts,
                const std::vector<vk::PushConstantRange> &pushConstantRanges);

        ~ComputePipeline();

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

        [[nodiscard]]
        const vk::PipelineLayout &getPipelineLayout() const;
    };

} // vklite

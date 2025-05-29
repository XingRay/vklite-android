//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <unordered_map>

#include "vklite/pipeline/pipeline_layout/PipelineLayout.h"
#include "vklite/pipeline/push_constants/PushConstantConfigure.h"
#include "vklite/pipeline/descriptor/DescriptorSetConfigure.h"

namespace vklite {

    class PipelineLayoutBuilder {
    private:
        // push constants
        std::vector<vk::PushConstantRange> mPushConstantRanges;

    public:
        PipelineLayoutBuilder();

        ~PipelineLayoutBuilder();

        PipelineLayoutBuilder &addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags);

        [[nodiscard]]
        PipelineLayout build(const Device &device, const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) const;

        [[nodiscard]]
        std::unique_ptr<PipelineLayout> buildUnique(const Device &device, const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) const;
    };

} // vklite

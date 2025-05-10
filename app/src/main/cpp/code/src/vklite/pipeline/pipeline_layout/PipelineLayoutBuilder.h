//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <unordered_map>

#include "vklite/pipeline/descriptor/old/DescriptorSlotConfigure.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayout.h"
#include "vklite/configure/push_constants/PushConstantConfigure.h"
#include "vklite/pipeline/descriptor/DescriptorSetConfigure.h"

namespace vklite {

    class PipelineLayoutBuilder {
    private:
        // set -> DescriptorSetConfigure
        std::unordered_map<uint32_t, DescriptorSetConfigure> mDescriptorSetConfigures;

        // push constants
        std::vector<vk::PushConstantRange> mPushConstantRanges;

    public:
        PipelineLayoutBuilder();

        ~PipelineLayoutBuilder();

        PipelineLayoutBuilder &addDescriptorSetConfigure(DescriptorSetConfigure &&descriptorSetConfigure);

        PipelineLayoutBuilder &addDescriptorSetConfigure(std::function<void(DescriptorSetConfigure &)> configure);

        PipelineLayoutBuilder &addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags);

        [[nodiscard]]
        PipelineLayout build(const Device &device);

        [[nodiscard]]
        std::unique_ptr<PipelineLayout> buildUnique(const Device &device);

    private:
        [[nodiscard]]
        std::vector<std::vector<vk::DescriptorSetLayoutBinding>> createDescriptorSetLayoutBindings();
    };

} // vklite

//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <unordered_map>

#include "DescriptorSlotConfigure.h"
//#include "vklite/pipeline/descriptor/DescriptorBindingSets.h"
#include "vklite/pipeline/descriptor/PipelineLayout.h"
#include "vklite/configure/push_constants/PushConstantConfigure.h"
#include "vklite/pipeline/descriptor/DescriptorSetConfigure.h"

namespace vklite {

    class PipelineLayoutConfigure {
    private:
        // set -> DescriptorSetConfigure
        std::unordered_map<uint32_t, DescriptorSetConfigure> mDescriptorSetConfigures;

        // push constants
        std::vector<vk::PushConstantRange> mPushConstantRanges;

    public:
        PipelineLayoutConfigure();

        ~PipelineLayoutConfigure();

        PipelineLayoutConfigure & addDescriptorSetConfigure(DescriptorSetConfigure&& descriptorSetConfigure);

        PipelineLayoutConfigure &addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags);

        [[nodiscard]]
        std::unique_ptr<PipelineLayout> createPipelineLayout(const Device &device);
    };

} // vklite

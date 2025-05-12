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

    class DescriptorConfigure {
    private:
        // set -> DescriptorSetConfigure
        std::unordered_map<uint32_t, DescriptorSetConfigure> mDescriptorSetConfigures;

    public:
        DescriptorConfigure();

        ~DescriptorConfigure();

        DescriptorConfigure &addDescriptorSetConfigure(DescriptorSetConfigure &&descriptorSetConfigure);

        DescriptorConfigure &addDescriptorSetConfigure(std::function<void(DescriptorSetConfigure &)> configure);

        [[nodiscard]]
        uint32_t getDescriptorSetCount() const;

        [[nodiscard]]
        std::vector<vk::DescriptorPoolSize> calcDescriptorPoolSizes() const;

    private:
        [[nodiscard]]
        std::vector<vk::DescriptorSetLayout> createDescriptorSetLayouts(const Device &device) const;
    };

} // vklite

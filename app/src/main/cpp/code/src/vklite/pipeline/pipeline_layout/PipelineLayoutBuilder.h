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
        std::unique_ptr<DescriptorPool> createDescriptorPool(const Device &device, uint32_t frameCount) const;

        [[nodiscard]]
        std::vector<vk::DescriptorSetLayout> createDescriptorSetLayouts(const Device &device) const;

        [[nodiscard]]
        uint32_t calcTotalPushConstantSize() const;

        [[nodiscard]]
        uint32_t getDescriptorSetCount() const;

        [[nodiscard]]
        std::vector<vk::DescriptorPoolSize> calcDescriptorPoolSizes() const;

        [[nodiscard]]
        PipelineLayout buildPipelineLayout(const Device &device)const;

        [[nodiscard]]
        std::unique_ptr<PipelineLayout> buildUniquePipelineLayout(const Device &device)const;


    private:
//        [[nodiscard]]
//        std::vector<std::vector<vk::DescriptorSetLayoutBinding>> createDescriptorSetLayoutBindings();
    };

} // vklite

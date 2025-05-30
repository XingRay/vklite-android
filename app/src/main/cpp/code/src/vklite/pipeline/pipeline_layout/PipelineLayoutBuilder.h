//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <unordered_map>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/pipeline_layout/PipelineLayout.h"
#include "vklite/pipeline/push_constants/PushConstantConfigure.h"
#include "vklite/pipeline/descriptor/DescriptorSetConfigure.h"

namespace vklite {

    class PipelineLayoutBuilder {
    private:
        vk::Device mDevice;

        // push constants
        std::vector<vk::PushConstantRange> mPushConstantRanges;

        // DescriptorSetLayout
        std::vector<vk::DescriptorSetLayout> mDescriptorSetLayouts;

    public:
        PipelineLayoutBuilder();

        ~PipelineLayoutBuilder();

        PipelineLayoutBuilder &device(vk::Device device);

        PipelineLayoutBuilder &pushConstantRanges(std::vector<vk::PushConstantRange> pushConstantRanges);

        PipelineLayoutBuilder &addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags);

        PipelineLayoutBuilder &descriptorSetLayouts(const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts);

        PipelineLayoutBuilder &descriptorSetLayouts(std::vector<vk::DescriptorSetLayout>&& descriptorSetLayouts);

        [[nodiscard]]
        PipelineLayout build() const;

        [[nodiscard]]
        std::unique_ptr<PipelineLayout> buildUnique() const;
    };

} // vklite

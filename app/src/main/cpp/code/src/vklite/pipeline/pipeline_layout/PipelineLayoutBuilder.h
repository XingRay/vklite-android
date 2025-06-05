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
#include "vklite/pipeline/shader/ShaderConfigure.h"

namespace vklite {

    class PipelineLayoutBuilder {
    private:
        vk::Device mDevice;

        vk::PipelineLayoutCreateInfo mPipelineLayoutCreateInfo;

        // push constants
        std::vector<vk::PushConstantRange> mPushConstantRanges;

        // DescriptorSetLayout
//        std::vector<vk::DescriptorSetLayout> mDescriptorSetLayouts;

    public:
        PipelineLayoutBuilder();

        ~PipelineLayoutBuilder();

        PipelineLayoutBuilder &device(vk::Device device);

        PipelineLayoutBuilder &pushConstantRanges(std::vector<vk::PushConstantRange> pushConstantRanges);

        PipelineLayoutBuilder &addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags);

        PipelineLayoutBuilder &descriptorSetLayouts(const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts);

//        PipelineLayoutBuilder &descriptorSetLayouts(std::vector<vk::DescriptorSetLayout> &&descriptorSetLayouts);

//        PipelineLayoutBuilder &config(const ShaderConfigure &shaderConfigure);

        [[nodiscard]]
        PipelineLayout build();

        [[nodiscard]]
        std::unique_ptr<PipelineLayout> buildUnique();
    };

} // vklite

//
// Created by leixing on 2025/3/11.
//
#include <iterator>

#include "PipelineLayoutBuilder.h"

#include "vklite/device/Device.h"

namespace vklite {

    PipelineLayoutBuilder::PipelineLayoutBuilder() = default;

    PipelineLayoutBuilder::~PipelineLayoutBuilder() = default;

    PipelineLayoutBuilder &PipelineLayoutBuilder::addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags) {
        vk::PushConstantRange pushConstantRange(stageFlags, offset, size);
        mPushConstantRanges.push_back(pushConstantRange);
        return *this;
    }

    PipelineLayout PipelineLayoutBuilder::build(const Device &device, const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) const {
//        return PipelineLayout(device, descriptorSetLayouts, mPushConstantRanges);
        return {device, descriptorSetLayouts, mPushConstantRanges};
    }

    std::unique_ptr<PipelineLayout> PipelineLayoutBuilder::buildUnique(const Device &device, const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) const {
        return std::make_unique<PipelineLayout>(device, descriptorSetLayouts, mPushConstantRanges);
    }

} // vklite
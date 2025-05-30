//
// Created by leixing on 2025/3/11.
//
#include <iterator>

#include "PipelineLayoutBuilder.h"

#include "vklite/device/Device.h"

namespace vklite {

    PipelineLayoutBuilder::PipelineLayoutBuilder() = default;

    PipelineLayoutBuilder::~PipelineLayoutBuilder() = default;

    PipelineLayoutBuilder &PipelineLayoutBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    PipelineLayoutBuilder &PipelineLayoutBuilder::pushConstantRanges(std::vector<vk::PushConstantRange> pushConstantRanges) {
        mPushConstantRanges = std::move(pushConstantRanges);
        return *this;
    }

    PipelineLayoutBuilder &PipelineLayoutBuilder::addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags) {
        vk::PushConstantRange pushConstantRange(stageFlags, offset, size);
        mPushConstantRanges.push_back(pushConstantRange);
        return *this;
    }

    PipelineLayoutBuilder &PipelineLayoutBuilder::descriptorSetLayouts(const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) {
        mDescriptorSetLayouts = descriptorSetLayouts;
        return *this;
    }

    PipelineLayoutBuilder &PipelineLayoutBuilder::descriptorSetLayouts(std::vector<vk::DescriptorSetLayout> &&descriptorSetLayouts) {
        mDescriptorSetLayouts = std::move(descriptorSetLayouts);
        return *this;
    }

    PipelineLayout PipelineLayoutBuilder::build() const {
        if (mDevice == nullptr) {
            throw std::runtime_error("mDevice == nullptr, must invoke PipelineLayoutBuilder::device(vk::Device device)");
        }

        vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo
                .setSetLayouts(mDescriptorSetLayouts)
                .setPushConstantRanges(mPushConstantRanges);

        vk::PipelineLayout pipelineLayout = mDevice.createPipelineLayout(pipelineLayoutCreateInfo);

//        return PipelineLayout(mDevice, pipelineLayout);
        return {mDevice, pipelineLayout};
    }

    std::unique_ptr<PipelineLayout> PipelineLayoutBuilder::buildUnique() const {
        return std::make_unique<PipelineLayout>(build());
    }

} // vklite
//
// Created by leixing on 2025/4/18.
//

#include "PipelineLayout.h"

namespace vklite {

    PipelineLayout::PipelineLayout(const Device &device,
                                   const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts,
                                   const std::vector<vk::PushConstantRange> &pushConstantRanges)
            : mDevice(device) {

        const vk::Device &vkDevice = mDevice.getDevice();

        vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo
                .setSetLayouts(descriptorSetLayouts)
                .setPushConstantRanges(pushConstantRanges);

        mPipelineLayout = device.getDevice().createPipelineLayout(pipelineLayoutCreateInfo);
    }

    PipelineLayout::~PipelineLayout() {
        const vk::Device &vkDevice = mDevice.getDevice();

        vkDevice.destroy(mPipelineLayout);
    }

    const vk::PipelineLayout &PipelineLayout::getPipelineLayout() const {
        return mPipelineLayout;
    }
} // vklite
//
// Created by leixing on 2025/5/11.
//

#include "DescriptorPoolBuilder.h"

namespace vklite {
    DescriptorPoolBuilder::DescriptorPoolBuilder()
            : mFrameCount(1) {}

    DescriptorPoolBuilder::~DescriptorPoolBuilder() = default;

    DescriptorPoolBuilder &DescriptorPoolBuilder::descriptorSetCount(uint32_t descriptorSetCount) {
        mDescriptorSetCount = descriptorSetCount;
        return *this;
    }

    DescriptorPoolBuilder &DescriptorPoolBuilder::descriptorPoolSizes(std::vector<vk::DescriptorPoolSize> &&descriptorPoolSizes) {
        mDescriptorPoolSizes = std::move(descriptorPoolSizes);
        return *this;
    }

    DescriptorPoolBuilder &DescriptorPoolBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    DescriptorPool DescriptorPoolBuilder::build(const Device &device) {
//        return DescriptorPool(device, calcDescriptorPoolSizes(mDescriptorPoolSizes, mFrameCount), mDescriptorSetCount * mFrameCount);
        return {device, calcDescriptorPoolSizes(mDescriptorPoolSizes, mFrameCount), mDescriptorSetCount * mFrameCount};
    }

    std::unique_ptr<DescriptorPool> DescriptorPoolBuilder::buildUnique(const Device &device) {
        return std::make_unique<DescriptorPool>(device, calcDescriptorPoolSizes(mDescriptorPoolSizes, mFrameCount), mDescriptorSetCount * mFrameCount);
    }

    std::vector<vk::DescriptorPoolSize> DescriptorPoolBuilder::calcDescriptorPoolSizes(const std::vector<vk::DescriptorPoolSize> &descriptorPoolSizes, uint32_t framerCount) {
        std::vector<vk::DescriptorPoolSize> poolSizes;
        for (const vk::DescriptorPoolSize &descriptorPoolSize: descriptorPoolSizes) {
            poolSizes.emplace_back(descriptorPoolSize.type, descriptorPoolSize.descriptorCount * framerCount);
        }
        return poolSizes;
    }

} // vklite
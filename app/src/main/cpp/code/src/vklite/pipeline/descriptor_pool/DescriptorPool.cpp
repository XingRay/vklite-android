//
// Created by leixing on 2025/3/12.
//

#include "DescriptorPool.h"

namespace vklite {

    DescriptorPool::DescriptorPool(const Device &device, std::vector<vk::DescriptorPoolSize> descriptorPoolSizes, uint32_t maxSets)
            : mDevice(device) {

        vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
        descriptorPoolCreateInfo
                .setPoolSizes(descriptorPoolSizes)
                .setMaxSets(maxSets)
                .setFlags(vk::DescriptorPoolCreateFlags{});

        mDescriptorPool = mDevice.getDevice().createDescriptorPool(descriptorPoolCreateInfo);
    }

    DescriptorPool::~DescriptorPool() {
        mDevice.getDevice().destroy(mDescriptorPool);
    }

    const vk::DescriptorPool &DescriptorPool::getDescriptorPool() const {
        return mDescriptorPool;
    }

    std::vector<vk::DescriptorSet> DescriptorPool::allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) {
        vk::DescriptorSetAllocateInfo allocateInfo{};
        allocateInfo
                .setDescriptorPool(mDescriptorPool)
                .setSetLayouts(descriptorSetLayouts);

        return mDevice.getDevice().allocateDescriptorSets(allocateInfo);
    }

} // vklite
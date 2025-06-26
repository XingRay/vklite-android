//
// Created by leixing on 2025/3/12.
//

#include "DescriptorPool.h"

#include <utility>

#include "vklite/Log.h"

namespace vklite {

    DescriptorPool::DescriptorPool(vk::Device device, vk::DescriptorPool descriptorPool)
            : mDevice(device), mDescriptorPool(descriptorPool) {}

    DescriptorPool::~DescriptorPool() {
        if (mDevice != nullptr && mDescriptorPool != nullptr) {
            LOG_D("mDevice.destroy(mDescriptorPool); mDescriptorPool:%p", (void *) mDescriptorPool);
            mDevice.destroy(mDescriptorPool);
            mDevice = nullptr;
            mDescriptorPool = nullptr;
        }
    }

    DescriptorPool::DescriptorPool(DescriptorPool &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mDescriptorPool(std::exchange(other.mDescriptorPool, nullptr)) {}

    DescriptorPool &DescriptorPool::operator=(DescriptorPool &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mDescriptorPool = std::exchange(other.mDescriptorPool, nullptr);
        }
        return *this;
    }

    const vk::DescriptorPool &DescriptorPool::getVkDescriptorPool() const {
        return mDescriptorPool;
    }

    std::vector<vk::DescriptorSet> DescriptorPool::allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) {
        vk::DescriptorSetAllocateInfo allocateInfo{};
        allocateInfo
                .setDescriptorPool(mDescriptorPool)
                .setSetLayouts(descriptorSetLayouts);

        return mDevice.allocateDescriptorSets(allocateInfo);
    }

} // vklite
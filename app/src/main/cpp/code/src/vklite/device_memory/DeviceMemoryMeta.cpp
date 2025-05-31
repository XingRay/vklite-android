//
// Created by leixing on 2025/6/1.
//

#include "DeviceMemoryMeta.h"

namespace vklite {

    DeviceMemoryMeta::DeviceMemoryMeta(vk::DeviceSize allocationSize, uint32_t memoryTypeIndex)
            : mAllocationSize(allocationSize), mMemoryTypeIndex(memoryTypeIndex) {}

    DeviceMemoryMeta::~DeviceMemoryMeta() = default;

    DeviceMemoryMeta::DeviceMemoryMeta(const DeviceMemoryMeta &other) = default;

    DeviceMemoryMeta &DeviceMemoryMeta::operator=(const DeviceMemoryMeta &other) {
        if (this != &other) {
            mAllocationSize = other.mAllocationSize;
            mMemoryTypeIndex = other.mMemoryTypeIndex;
        }
        return *this;
    }

    DeviceMemoryMeta::DeviceMemoryMeta(DeviceMemoryMeta &&other) noexcept
            : mAllocationSize(other.mAllocationSize), mMemoryTypeIndex(other.mMemoryTypeIndex) {}

    DeviceMemoryMeta &DeviceMemoryMeta::operator=(DeviceMemoryMeta &&other) noexcept {
        if (this != &other) {
            mAllocationSize = other.mAllocationSize;
            mMemoryTypeIndex = other.mMemoryTypeIndex;
        }
        return *this;
    }

    vk::DeviceSize DeviceMemoryMeta::getAllocationSize() const {
        return mAllocationSize;
    }

    uint32_t DeviceMemoryMeta::getMemoryTypeIndex() const {
        return mMemoryTypeIndex;
    }

} // vklite
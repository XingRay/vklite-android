//
// Created by leixing on 2025/6/1.
//

#include "StorageBufferBuilder.h"

namespace vklite {

    StorageBufferBuilder::StorageBufferBuilder() {
        mCombinedMemoryBufferBuilder.asDeviceLocal().addUsage(vk::BufferUsageFlagBits::eStorageBuffer);
    };

    StorageBufferBuilder::~StorageBufferBuilder() = default;

    StorageBufferBuilder &StorageBufferBuilder::device(vk::Device device) {
        mDevice = device;
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    StorageBufferBuilder &StorageBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    StorageBufferBuilder &StorageBufferBuilder::addUsage(vk::BufferUsageFlags usage) {
        mCombinedMemoryBufferBuilder.addUsage(usage);
        return *this;
    }

    StorageBufferBuilder &StorageBufferBuilder::configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    StorageBufferBuilder &StorageBufferBuilder::configDeviceMemory(vk::PhysicalDevice physicalDevice) {
        configDeviceMemory(physicalDevice.getMemoryProperties());
        return *this;
    }

    StorageBuffer StorageBufferBuilder::build() {
//        return StorageBuffer(mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties);
        return {mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties};
    }

    std::unique_ptr<StorageBuffer> StorageBufferBuilder::buildUnique() {
        return std::make_unique<StorageBuffer>(build());
    }

} // vklite
//
// Created by leixing on 2025/5/31.
//

#include "DeviceLocalBufferBuilder.h"

namespace vklite {

    DeviceLocalBufferBuilder::DeviceLocalBufferBuilder() {
        mCombinedMemoryBufferBuilder.asDeviceLocal();
    };

    DeviceLocalBufferBuilder::~DeviceLocalBufferBuilder() = default;

    DeviceLocalBufferBuilder &DeviceLocalBufferBuilder::device(vk::Device device) {
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    DeviceLocalBufferBuilder &DeviceLocalBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    DeviceLocalBufferBuilder &DeviceLocalBufferBuilder::addUsage(vk::BufferUsageFlags usage) {
        mCombinedMemoryBufferBuilder.addUsage(usage);
        return *this;
    }

    DeviceLocalBufferBuilder &DeviceLocalBufferBuilder::configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    DeviceLocalBufferBuilder &DeviceLocalBufferBuilder::configDeviceMemory(vk::PhysicalDevice physicalDevice) {
        configDeviceMemory(physicalDevice.getMemoryProperties());
        return *this;
    }

    DeviceLocalBuffer DeviceLocalBufferBuilder::build() {
        return mCombinedMemoryBufferBuilder.build();
    }

    std::unique_ptr<DeviceLocalBuffer> DeviceLocalBufferBuilder::buildUnique() {
        return std::make_unique<DeviceLocalBuffer>(build());
    }

} // vklite
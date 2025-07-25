//
// Created by leixing on 2025/5/31.
//

#include "DeviceLocalBufferBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    DeviceLocalBufferBuilder::DeviceLocalBufferBuilder() {
        mCombinedMemoryBufferBuilder.asDeviceLocal();
    };

    DeviceLocalBufferBuilder::~DeviceLocalBufferBuilder() = default;

    DeviceLocalBufferBuilder &DeviceLocalBufferBuilder::device(vk::Device device) {
        mDevice = device;
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

    DeviceLocalBufferBuilder &DeviceLocalBufferBuilder::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    DeviceLocalBuffer DeviceLocalBufferBuilder::build() {
        LOG_D("DeviceLocalBufferBuilder::build()");
        if (mDevice == nullptr) {
            throw std::runtime_error("DeviceLocalBufferBuilder::build(): mDevice == nullptr");
        }
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("DeviceLocalBufferBuilder::build(): mPhysicalDeviceMemoryProperties not set");
        }
        return DeviceLocalBuffer{mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties};
    }

    std::unique_ptr<DeviceLocalBuffer> DeviceLocalBufferBuilder::buildUnique() {
        return std::make_unique<DeviceLocalBuffer>(build());
    }

    std::vector<DeviceLocalBuffer> DeviceLocalBufferBuilder::build(uint32_t count) {
        std::vector<DeviceLocalBuffer> buffers;
        buffers.reserve(count);

        for (int i = 0; i < count; i++) {
            buffers.emplace_back(mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties);
        }

        return buffers;
    }

} // vklite
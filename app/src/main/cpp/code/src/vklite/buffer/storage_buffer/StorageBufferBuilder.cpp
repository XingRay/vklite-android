//
// Created by leixing on 2025/6/1.
//

#include "StorageBufferBuilder.h"
#include "vklite/Log.h"

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

    StorageBufferBuilder &StorageBufferBuilder::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    StorageBuffer StorageBufferBuilder::build() {
        LOG_D("StorageBufferBuilder::build()");
        if (mDevice == nullptr) {
            throw std::runtime_error("StorageBufferBuilder::build(): mDevice == nullptr");
        }
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("StorageBufferBuilder::build(): mPhysicalDeviceMemoryProperties not set");
        }
//        return StorageBuffer(mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties);
        return {mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties};
    }

    std::unique_ptr<StorageBuffer> StorageBufferBuilder::buildUnique() {
        return std::make_unique<StorageBuffer>(build());
    }

    std::vector<StorageBuffer> StorageBufferBuilder::build(uint32_t count) {
        std::vector<StorageBuffer> buffers;
        buffers.reserve(count);

        for (int i = 0; i < count; i++) {
            buffers.emplace_back(mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties);
        }

        return buffers;
    }

} // vklite
//
// Created by leixing on 2025/3/22.
//

#include "IndexBufferBuilder.h"
#include "vklite/buffer/index_buffer/IndexBufferMeta.h"

namespace vklite {

    IndexBufferBuilder::IndexBufferBuilder()
            : mIndexType(vk::IndexType::eUint32) {
        mCombinedMemoryBufferBuilder.asDeviceLocal().addUsage(vk::BufferUsageFlagBits::eIndexBuffer);
    }

    IndexBufferBuilder::~IndexBufferBuilder() = default;

    IndexBufferBuilder &IndexBufferBuilder::device(vk::Device device) {
        mDevice = device;
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    IndexBufferBuilder &IndexBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    IndexBufferBuilder &IndexBufferBuilder::configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    IndexBufferBuilder &IndexBufferBuilder::configDeviceMemory(vk::PhysicalDevice physicalDevice) {
        configDeviceMemory(physicalDevice.getMemoryProperties());
        return *this;
    }

    IndexBufferBuilder &IndexBufferBuilder::indexType(vk::IndexType indexType) {
        mIndexType = indexType;
        return *this;
    }

    IndexBuffer IndexBufferBuilder::build() {
        return {mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties, {mIndexType, 0}};
    }

    std::unique_ptr<IndexBuffer> IndexBufferBuilder::buildUnique() {
        return std::make_unique<IndexBuffer>(build());
    }

} // vklite
//
// Created by leixing on 2025/5/31.
//

#include "HostVisibleBufferBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    HostVisibleBufferBuilder::HostVisibleBufferBuilder() {
        mCombinedMemoryBufferBuilder.asHostVisible();
    }

    HostVisibleBufferBuilder::~HostVisibleBufferBuilder() = default;

    HostVisibleBufferBuilder &HostVisibleBufferBuilder::device(vk::Device device) {
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    HostVisibleBufferBuilder &HostVisibleBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    HostVisibleBufferBuilder &HostVisibleBufferBuilder::addUsage(vk::BufferUsageFlags usage) {
        mCombinedMemoryBufferBuilder.addUsage(usage);
        return *this;
    }

    HostVisibleBufferBuilder &HostVisibleBufferBuilder::configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    HostVisibleBufferBuilder &HostVisibleBufferBuilder::configDeviceMemory(vk::PhysicalDevice physicalDevice) {
        configDeviceMemory(physicalDevice.getMemoryProperties());
        return *this;
    }

    HostVisibleBuffer HostVisibleBufferBuilder::build() {
        LOG_D("HostVisibleBufferBuilder::build()");
        return HostVisibleBuffer(mCombinedMemoryBufferBuilder.build());
    }

    std::unique_ptr<HostVisibleBuffer> HostVisibleBufferBuilder::buildUnique() {
        return std::make_unique<HostVisibleBuffer>(build());
    }

    std::vector<HostVisibleBuffer> HostVisibleBufferBuilder::build(uint32_t count) {
        std::vector<HostVisibleBuffer> buffers;
        buffers.reserve(count);

        for (int i = 0; i < count; i++) {
            buffers.emplace_back(mCombinedMemoryBufferBuilder.build());
        }

        return buffers;
    }

} // vklite
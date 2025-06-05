//
// Created by leixing on 2025/5/31.
//

#include "StagingBufferBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    StagingBufferBuilder::StagingBufferBuilder() {
        mCombinedMemoryBufferBuilder.asHostVisible();
    }

    StagingBufferBuilder::~StagingBufferBuilder() = default;

    StagingBufferBuilder &StagingBufferBuilder::device(vk::Device device) {
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    StagingBufferBuilder &StagingBufferBuilder::configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    StagingBufferBuilder &StagingBufferBuilder::configDeviceMemory(vk::PhysicalDevice physicalDevice) {
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDevice.getMemoryProperties());
        return *this;
    }

    StagingBufferBuilder &StagingBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    StagingBuffer StagingBufferBuilder::build() {
        LOG_D("StagingBufferBuilder::build()");
//        return StagingBuffer(mCombinedMemoryBufferBuilder.build());
        return StagingBuffer(mCombinedMemoryBufferBuilder.build());
    }

    std::unique_ptr<StagingBuffer> StagingBufferBuilder::buildUnique() {
        return std::make_unique<StagingBuffer>(build());
    }

    std::vector<StagingBuffer> StagingBufferBuilder::build(uint32_t count) {
        std::vector<StagingBuffer> buffers;
        buffers.reserve(count);

        for (int i = 0; i < count; i++) {
            buffers.emplace_back(mCombinedMemoryBufferBuilder.build());
        }

        return buffers;
    }

} // vklite
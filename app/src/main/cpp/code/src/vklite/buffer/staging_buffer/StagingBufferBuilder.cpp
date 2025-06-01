//
// Created by leixing on 2025/5/31.
//

#include "StagingBufferBuilder.h"

namespace vklite {

    StagingBufferBuilder::StagingBufferBuilder() {
        mCombinedMemoryBufferBuilder.asHostVisible();
    }

    StagingBufferBuilder::~StagingBufferBuilder() = default;

    StagingBufferBuilder &StagingBufferBuilder::device(vk::Device device) {
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    StagingBufferBuilder &StagingBufferBuilder::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    StagingBufferBuilder &StagingBufferBuilder::physicalDeviceMemoryProperties(vk::PhysicalDevice physicalDevice) {
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDevice.getMemoryProperties());
        return *this;
    }

    StagingBufferBuilder &StagingBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    StagingBuffer StagingBufferBuilder::build() {
//        return StagingBuffer(mCombinedMemoryBufferBuilder.build());
        return StagingBuffer(mCombinedMemoryBufferBuilder.build());
    }

    std::unique_ptr<StagingBuffer> StagingBufferBuilder::buildUnique() {
        return std::make_unique<StagingBuffer>(build());
    }

} // vklite
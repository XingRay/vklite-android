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
        mDevice = device;
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    StagingBufferBuilder &StagingBufferBuilder::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    StagingBufferBuilder &StagingBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    StagingBuffer StagingBufferBuilder::build() {
//        LOG_D("StagingBufferBuilder::build()");
        if (mDevice == nullptr) {
            throw std::runtime_error("StagingBufferBuilder::build(): mDevice == nullptr");
        }
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("StagingBufferBuilder::build(): mPhysicalDeviceMemoryProperties not set");
        }
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
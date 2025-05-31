//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <memory>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBufferBuilder.h"
#include "vklite/buffer/staging_buffer/StagingBuffer.h"

namespace vklite {

    class StagingBufferBuilder {
    private:
        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;

    public:
        StagingBufferBuilder();

        ~StagingBufferBuilder();

        StagingBufferBuilder &device(vk::Device device);

        StagingBufferBuilder &physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        StagingBufferBuilder &physicalDeviceMemoryProperties(vk::PhysicalDevice physicalDevice);

        StagingBufferBuilder &size(vk::DeviceSize size);

        StagingBuffer build();

        std::unique_ptr<StagingBuffer> buildUnique();
    };

} // vklite

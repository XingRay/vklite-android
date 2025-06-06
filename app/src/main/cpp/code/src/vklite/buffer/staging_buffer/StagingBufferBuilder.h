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
        vk::Device mDevice;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;

    public:
        StagingBufferBuilder();

        ~StagingBufferBuilder();

        StagingBufferBuilder &device(vk::Device device);

        StagingBufferBuilder &physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        StagingBufferBuilder &size(vk::DeviceSize size);

        [[nodiscard]]
        StagingBuffer build();

        [[nodiscard]]
        std::unique_ptr<StagingBuffer> buildUnique();

        [[nodiscard]]
        std::vector<StagingBuffer> build(uint32_t count);
    };

} // vklite

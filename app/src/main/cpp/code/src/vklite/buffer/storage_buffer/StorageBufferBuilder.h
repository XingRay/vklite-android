//
// Created by leixing on 2025/6/1.
//

#pragma once

#include <memory>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBufferBuilder.h"
#include "vklite/buffer/storage_buffer/StorageBuffer.h"

namespace vklite {

    class StorageBufferBuilder {
    private:
        vk::Device mDevice;
        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

    public:
        StorageBufferBuilder();

        ~StorageBufferBuilder();

        StorageBufferBuilder &device(vk::Device device);

        StorageBufferBuilder &size(vk::DeviceSize size);

        StorageBufferBuilder &addUsage(vk::BufferUsageFlags usage);

        StorageBufferBuilder &configDeviceMemory(vk::PhysicalDevice physicalDevice);

        StorageBufferBuilder &configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        [[nodiscard]]
        StorageBuffer build();

        [[nodiscard]]
        std::unique_ptr<StorageBuffer> buildUnique();
    };

} // vklite
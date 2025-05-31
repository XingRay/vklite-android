//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <memory>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBufferBuilder.h"
#include "vklite/buffer/host_visible/HostVisibleBuffer.h"

namespace vklite {

    class HostVisibleBufferBuilder {
    private:
        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;

    public:
        HostVisibleBufferBuilder();

        ~HostVisibleBufferBuilder();

        HostVisibleBufferBuilder &device(vk::Device device);

        HostVisibleBufferBuilder &size(vk::DeviceSize size);

        HostVisibleBufferBuilder &addUsage(vk::BufferUsageFlags usage);

        HostVisibleBufferBuilder &configDeviceMemory(vk::PhysicalDevice physicalDevice);

        HostVisibleBufferBuilder &configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        HostVisibleBuffer build();

        std::unique_ptr<HostVisibleBuffer> buildUnique();
    };

} // vklite

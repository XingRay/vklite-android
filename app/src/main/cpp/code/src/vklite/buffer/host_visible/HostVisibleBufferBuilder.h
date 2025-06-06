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
        vk::Device mDevice;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;

    public:
        HostVisibleBufferBuilder();

        ~HostVisibleBufferBuilder();

        HostVisibleBufferBuilder &device(vk::Device device);

        HostVisibleBufferBuilder &size(vk::DeviceSize size);

        HostVisibleBufferBuilder &addUsage(vk::BufferUsageFlags usage);

        HostVisibleBufferBuilder &physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        [[nodiscard]]
        HostVisibleBuffer build();

        [[nodiscard]]
        std::unique_ptr<HostVisibleBuffer> buildUnique();

        [[nodiscard]]
        std::vector<HostVisibleBuffer> build(uint32_t count);
    };

} // vklite

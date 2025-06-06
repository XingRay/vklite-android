//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <memory>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBufferBuilder.h"
#include "vklite/buffer/device_local/DeviceLocalBuffer.h"

namespace vklite {

    class DeviceLocalBufferBuilder {
    private:
        vk::Device mDevice;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;

    public:
        DeviceLocalBufferBuilder();

        ~DeviceLocalBufferBuilder();

        DeviceLocalBufferBuilder &device(vk::Device device);

        DeviceLocalBufferBuilder &size(vk::DeviceSize size);

        DeviceLocalBufferBuilder &addUsage(vk::BufferUsageFlags usage);

        DeviceLocalBufferBuilder &physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        [[nodiscard]]
        DeviceLocalBuffer build();

        [[nodiscard]]
        std::unique_ptr<DeviceLocalBuffer> buildUnique();

        [[nodiscard]]
        std::vector<DeviceLocalBuffer> build(uint32_t count);
    };

} // vklite

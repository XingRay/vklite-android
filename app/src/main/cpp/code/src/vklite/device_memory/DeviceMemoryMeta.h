//
// Created by leixing on 2025/6/1.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class DeviceMemoryMeta {
    private:
        vk::DeviceSize mAllocationSize;
        uint32_t mMemoryTypeIndex;

    public:
        explicit DeviceMemoryMeta(vk::DeviceSize allocationSize, uint32_t memoryTypeIndex);

        ~DeviceMemoryMeta();

        DeviceMemoryMeta(const DeviceMemoryMeta &other);

        DeviceMemoryMeta &operator=(const DeviceMemoryMeta &other);

        DeviceMemoryMeta(DeviceMemoryMeta &&other) noexcept;

        DeviceMemoryMeta &operator=(DeviceMemoryMeta &&other) noexcept;

        vk::DeviceSize getAllocationSize() const;

        uint32_t getMemoryTypeIndex() const;
    };

} // vklite

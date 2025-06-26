//
// Created by leixing on 2025/5/30.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/device_memory/DeviceMemoryMeta.h"

namespace vklite {

    class DeviceMemory {
    private:
        vk::Device mDevice;
        vk::DeviceMemory mDeviceMemory;
        DeviceMemoryMeta mMeta;

    public:
        explicit DeviceMemory(const vk::Device &device, const vk::DeviceMemory &deviceMemory, DeviceMemoryMeta &&meta);

        ~DeviceMemory();

        DeviceMemory(const DeviceMemory &other) = delete;

        DeviceMemory &operator=(const DeviceMemory &other) = delete;

        DeviceMemory(DeviceMemory &&other) noexcept;

        DeviceMemory &operator=(DeviceMemory &&other) noexcept;

        [[nodiscard]]
        const vk::DeviceMemory &getVkDeviceMemory() const;

        void updateBuffer(const void *data, uint32_t size, uint32_t offset, vk::MemoryMapFlags flags = vk::MemoryMapFlags{}) const;

        void updateBuffer(const void *data, uint32_t size) const;
    };

} // vklite

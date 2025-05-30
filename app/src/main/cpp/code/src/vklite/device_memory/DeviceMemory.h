//
// Created by leixing on 2025/5/30.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class DeviceMemory {
    private:
        vk::Device mDevice;
        vk::DeviceMemory mDeviceMemory;

    public:
        DeviceMemory(const vk::Device &device, const vk::DeviceMemory &deviceMemory);

        ~DeviceMemory();

        DeviceMemory(const DeviceMemory &other) = delete;

        DeviceMemory &operator=(const DeviceMemory &other) = delete;

        DeviceMemory(DeviceMemory &&other) noexcept;

        DeviceMemory &operator=(DeviceMemory &&other) noexcept;

        [[nodiscard]]
        const vk::DeviceMemory& getDeviceMemory()const;
    };

} // vklite

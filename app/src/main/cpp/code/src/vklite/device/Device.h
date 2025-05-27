//
// Created by leixing on 2024/12/23.
//

#pragma once

#include <optional>

#include <vulkan/vulkan.hpp>

#include "vklite/util/selector/string/StringListSelector.h"
#include "vklite/swapchain/SwapChainSupportDetail.h"
#include "vklite/physical_device/QueueFamilyIndices.h"
#include "vklite/physical_device/PhysicalDevice.h"
#include "vklite/device/DevicePluginInterface.h"

namespace vklite {

    class Device {
    private:
        vk::Device mDevice;

    public:
        explicit Device(vk::Device device);

        ~Device();

        Device(const Device &other) = delete;

        Device &operator=(const Device &other) = delete;

        Device(Device &&other) noexcept;

        Device &operator=(Device &&other) noexcept;

        [[nodiscard]]
        const vk::Device &getDevice() const;

        [[nodiscard]]
        vk::Queue getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex = 0) const;
    };
} // vklite

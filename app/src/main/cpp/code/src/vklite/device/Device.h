//
// Created by leixing on 2024/12/23.
//

#pragma once

#include <optional>

#include <vulkan/vulkan.hpp>

#include "vklite/util/selector/StringListSelector.h"
#include "vklite/swapchain/SwapChainSupportDetail.h"
#include "vklite/physical_device/QueueFamilyIndices.h"
#include "vklite/physical_device/PhysicalDevice.h"
#include "vklite/device/DevicePlugin.h"

namespace vklite {

    class Device {
    private:
        const PhysicalDevice &mPhysicalDevice;
        vk::Device mDevice;

        std::unordered_map<vk::QueueFlagBits, std::vector<uint32_t>> mQueueFamilyIndicesMap;
        std::vector<uint32_t> mPresentQueueFamilyIndices;

        std::vector<uint32_t> mQueueFamilyIndices;

        vk::Queue mGraphicsQueue;
        uint32_t mGraphicQueueFamilyIndex;

        vk::Queue mPresentQueue;
        uint32_t mPresentQueueFamilyIndex;

        vk::Queue mComputeQueue;
        uint32_t mComputeQueueFamilyIndex;

    public:
        Device(const PhysicalDevice &physicalDevice,
               std::unordered_map<vk::QueueFlagBits, std::vector<uint32_t>> &queueFamilyIndicesMap,
               std::vector<uint32_t> &presentQueueFamilyIndices,
               std::vector<const char *> &&extensions,
               std::vector<const char *> &&layers,
               const vk::PhysicalDeviceFeatures &physicalDeviceFeatures,
               const std::vector<std::unique_ptr<DevicePlugin>> &devicePlugins);

        ~Device();

        [[nodiscard]]
        const PhysicalDevice &getPhysicalDevice() const;

        [[nodiscard]]
        const vk::Device &getDevice() const;

        [[nodiscard]]
        uint32_t getGraphicQueueFamilyIndex() const;

        [[nodiscard]]
        uint32_t getPresentQueueFamilyIndex() const;

        [[nodiscard]]
        const std::vector<uint32_t> &getQueueFamilyIndices() const;

        [[nodiscard]]
        vk::Queue getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex = 0) const;

        [[nodiscard]]
        const vk::Queue &getGraphicsQueue() const;

        [[nodiscard]]
        const vk::Queue &getPresentQueue() const;

    };
} // vklite

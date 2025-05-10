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
        const vk::Queue &getGraphicsQueue() const;

        [[nodiscard]]
        const vk::Queue &getPresentQueue() const;

        [[nodiscard]]
        std::pair<vk::Buffer, vk::DeviceMemory> createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) const;

        [[nodiscard]]
        std::pair<vk::Image, vk::DeviceMemory> createImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::SampleCountFlagBits numSamples, vk::Format format,
                                                           vk::ImageTiling imageTiling, vk::ImageUsageFlags imageUsage, vk::MemoryPropertyFlags memoryProperty) const;

        [[nodiscard]]
        vk::ImageView createImageView(const vk::Image &image, vk::Format format, vk::ImageAspectFlags imageAspect, uint32_t levelCount) const;

    };
} // vklite

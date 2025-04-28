//
// Created by leixing on 2024/12/23.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include <optional>

#include "vklite/util/selector/StringListSelector.h"
#include "vklite/swapchain/SwapChainSupportDetail.h"
#include "vklite/physical_device/QueueFamilyIndices.h"
#include "vklite/physical_device/PhysicalDeviceSurfaceSupport.h"
#include "vklite/physical_device/PhysicalDevice.h"

namespace vklite {

    class Device {
    private:
        const PhysicalDevice &mPhysicalDevice;
        vk::Device mDevice;

        uint32_t mGraphicQueueFamilyIndex;
        uint32_t mPresentQueueFamilyIndex;
        std::vector<uint32_t> mQueueFamilyIndices;

        vk::Queue mGraphicsQueue;
        vk::Queue mPresentQueue;

        vk::SurfaceCapabilitiesKHR mCapabilities;
        std::vector<vk::SurfaceFormatKHR> mFormats;
        std::vector<vk::PresentModeKHR> mPresentModes;

    public:
        Device(const PhysicalDevice &physicalDevice,
               const PhysicalDeviceSurfaceSupport &surfaceSupport,
               const std::vector<std::string> &deviceExtensions,
               const std::vector<std::string> &layers);

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
        vk::SurfaceCapabilitiesKHR getCapabilities() const;

        [[nodiscard]]
        std::vector<vk::SurfaceFormatKHR> getFormats() const;

        [[nodiscard]]
        std::vector<vk::PresentModeKHR> getPresentModes() const;

        [[nodiscard]]
        std::pair<vk::Buffer, vk::DeviceMemory> createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) const;

        [[nodiscard]]
        std::pair<vk::Image, vk::DeviceMemory> createImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::SampleCountFlagBits numSamples, vk::Format format,
                                                           vk::ImageTiling imageTiling, vk::ImageUsageFlags imageUsage, vk::MemoryPropertyFlags memoryProperty) const;

        [[nodiscard]]
        vk::ImageView createImageView(const vk::Image &image, vk::Format format, vk::ImageAspectFlags imageAspect, uint32_t mipLevels) const;

    };
} // vklite

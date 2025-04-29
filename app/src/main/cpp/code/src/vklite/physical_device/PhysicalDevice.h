//
// Created by leixing on 2025/1/9.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include <optional>

#include "PhysicalDeviceSurfaceSupport.h"
#include "vklite/surface/Surface.h"
#include "vklite/physical_device/QueueFamilyIndices.h"

namespace vklite {

    class PhysicalDevice {
    private:
        vk::PhysicalDevice mPhysicalDevice;

    public:

        explicit PhysicalDevice(const vk::PhysicalDevice &physicalDevice);

        ~PhysicalDevice();

        [[nodiscard]]
        std::optional<PhysicalDeviceSurfaceSupport> querySurfaceSupport(const Surface &surface, vk::QueueFlags requiredQueueFlags) const;

        [[nodiscard]]
        QueueFamilyIndices findQueueFamilies(const vk::SurfaceKHR &surface, vk::QueueFlags requiredFlags) const;

        [[nodiscard]]
        std::vector<uint32_t> findQueueFamilyIndicesByFlags(vk::QueueFlags requiredFlags) const;

        [[nodiscard]]
        std::vector<uint32_t> findQueueFamilyIndicesBySurface(const vk::SurfaceKHR &surface) const;

        [[nodiscard]]
        bool isSupportExtensions(const std::vector<std::string> &extensions) const;

        [[nodiscard]]
        const vk::PhysicalDevice &getPhysicalDevice() const;

        [[nodiscard]]
        vk::SampleCountFlagBits queryMaxUsableSampleCount() const;

        [[nodiscard]]
        std::vector<vk::SampleCountFlagBits> querySupportedSampleCountFlagBits() const;

        [[nodiscard]]
        std::vector<uint32_t> querySupportedSampleCounts() const;

        [[nodiscard]]
        std::string queryName() const;

        [[nodiscard]]
        vk::PhysicalDeviceType queryType() const;

        void printInfo() const;

        void printInfoWithSurface(const vk::SurfaceKHR &surface) const;

        [[nodiscard]]
        std::vector<std::string> queryExtensionNames() const;

        [[nodiscard]]
        uint32_t getMaxPushConstantsSize() const;

        [[nodiscard]]
        float getMaxSamplerAnisotropy() const;

        [[nodiscard]]
        uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;

        [[nodiscard]]
        uint32_t findMemoryType(uint32_t typeFilter) const;

        [[nodiscard]]
        vk::Format findSupportedFormat(const std::vector<vk::Format> &candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const;

        [[nodiscard]]
        vk::Format findDepthFormat() const;

        [[nodiscard]]
        bool isSupportFormatFeature(vk::Format format, vk::FormatFeatureFlags formatFeatureFlags) const;
    };

} // vklite

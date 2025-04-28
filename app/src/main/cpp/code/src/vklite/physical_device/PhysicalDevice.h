//
// Created by leixing on 2025/1/9.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include <optional>

#include "PhysicalDeviceSurfaceSupport.h"
#include "vklite/surface/Surface.h"

namespace vklite {

    class PhysicalDevice {
    private:
        vk::PhysicalDevice mPhysicalDevice;

    public:

        explicit PhysicalDevice(const vk::PhysicalDevice &physicalDevice);

        ~PhysicalDevice();

        [[nodiscard]]
        std::optional<PhysicalDeviceSurfaceSupport> querySurfaceSupport(const Surface &surface, vk::QueueFlags requiredQueueFlags) const;

        bool isSupportExtensions(const std::vector<std::string> &extensions) const;

        [[nodiscard]]
        const vk::PhysicalDevice &getPhysicalDevice() const;

        [[nodiscard]]
        vk::SampleCountFlagBits queryMaxUsableSampleCount() const;

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
    };

} // vklite

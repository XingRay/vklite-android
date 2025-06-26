//
// Created by leixing on 2025/1/9.
//

#pragma once

#include <vector>
#include <string>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class PhysicalDevice {
    private:
        vk::PhysicalDevice mPhysicalDevice;

    public:

        explicit PhysicalDevice(const vk::PhysicalDevice &physicalDevice);

        ~PhysicalDevice();

        PhysicalDevice(const PhysicalDevice &other) = delete;

        PhysicalDevice &operator=(const PhysicalDevice &other) = delete;

        PhysicalDevice(PhysicalDevice &&other) noexcept;

        PhysicalDevice &operator=(PhysicalDevice &&other) noexcept;

        [[nodiscard]]
        const vk::PhysicalDevice &getVkPhysicalDevice() const;

        [[nodiscard]]
        vk::PhysicalDeviceMemoryProperties getMemoryProperties() const;

        [[nodiscard]]
        vk::PhysicalDeviceProperties getProperties() const;

        [[nodiscard]]
        vk::PhysicalDeviceFeatures getFeatures() const;

        [[nodiscard]]
        std::vector<uint32_t> queryQueueFamilyIndicesByFlags(vk::QueueFlags requiredFlags) const;

        [[nodiscard]]
        std::vector<uint32_t> queryQueueFamilyIndicesBySurface(const vk::SurfaceKHR &surface) const;

        [[nodiscard]]
        bool isSupportExtensions(const std::vector<std::string> &extensions) const;

        [[nodiscard]]
        vk::SampleCountFlagBits queryMaxUsableSampleCount() const;

        [[nodiscard]]
        std::vector<vk::SampleCountFlagBits> querySampleCountFlagBits() const;

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

        [[nodiscard]]
        vk::SampleCountFlagBits selectMaxMsaaSampleCountFlagBits(uint32_t maxLimit = std::numeric_limits<uint32_t>::max()) const;

        [[nodiscard]]
        vk::SurfaceCapabilitiesKHR getCapabilities(const vk::SurfaceKHR &surface) const;

        [[nodiscard]]
        std::vector<vk::SurfaceFormatKHR> getFormats(const vk::SurfaceKHR &surface) const;

        [[nodiscard]]
        std::vector<vk::PresentModeKHR> getPresentModes(const vk::SurfaceKHR &surface) const;
    };

} // vklite

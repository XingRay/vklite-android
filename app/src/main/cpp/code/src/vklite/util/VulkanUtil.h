//
// Created by leixing on 2024/12/30.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>
#include "vklite/device/Device.h"

namespace vklite {

    class VulkanUtil {
    public:

//        static void recordCopyBufferCommand(const vk::CommandBuffer &commandBuffer, const vk::Buffer &srcBuffer, const vk::Buffer &dstBuffer, vk::DeviceSize size);

        static std::string sampleCountFlagsToString(vk::SampleCountFlags flags);

        static std::string formatDeviceSize(vk::DeviceSize size);

        static uint32_t calcMipLevles(uint32_t width, uint32_t height);

        static vk::ImageAspectFlags calcImageAspectFlags(vk::ImageLayout imageLayout, vk::Format format);

        static void recordTransitionImageLayoutCommand(const vk::CommandBuffer &commandBuffer,
                                                       vk::Image image,
                                                       vk::Format format,
                                                       vk::ImageLayout oldImageLayout,
                                                       vk::ImageLayout newImageLayout,
                                                       uint32_t mipLevels,
                                                       uint32_t srcQueueFamilyIndex,
                                                       uint32_t dstQueueFamilyIndex);

        static bool hasStencilComponent(vk::Format format);

        static vk::SampleCountFlagBits uint32ToSampleCountFlagBits(uint32_t sampleCount);

        static uint32_t sampleCountFlagBitsToUint32(vk::SampleCountFlagBits sampleCountFlagBits);

        static uint32_t getFormatSize(vk::Format format);

        static uint32_t getImageFormatBytesPerPixel(vk::Format format);

        static vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capability, uint32_t width, uint32_t height);

        static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);

        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);

        static uint32_t findMemoryTypeIndex(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::PhysicalDeviceMemoryProperties memoryProperties);

        static uint32_t findMemoryTypeIndex(vk::PhysicalDevice physicalDevice, vk::Device device, vk::Image image, vk::MemoryPropertyFlags memoryPropertyFlags);

        static uint32_t findMemoryTypeIndex(vk::PhysicalDeviceMemoryProperties memoryProperties,
                                            vk::MemoryRequirements memoryRequirements,
                                            vk::MemoryPropertyFlags memoryPropertyFlags);

        static uint32_t findMemoryTypeIndex(vk::PhysicalDeviceMemoryProperties memoryProperties, uint32_t typeFilter);

        static std::string toString(vk::ImageTiling imageTiling);

        static std::string toString(vk::ImageType imageType);

        static std::string toString(vk::ImageUsageFlags usage);

        static std::string toString(vk::MemoryPropertyFlags memoryProperty);

        static std::string toString(vk::PhysicalDeviceType deviceType);

        static std::string toString(vk::QueueFlags queueFlagBits);

        static std::string toString(vk::SampleCountFlags sampleCount);

        static std::string toString(vk::Format format);

        static std::string toString(vk::SharingMode sharingMode);

        static std::string toString(vk::ImageLayout imageLayout);

        static std::string toString(vk::ImageViewType imageViewType);

        static std::string toString(vk::ComponentSwizzle componentSwizzle);

        static std::string toString(vk::ImageAspectFlags imageAspect);

        static void printExtensions(const std::vector<vk::ExtensionProperties>& extensionProperties);
    };

} // vklite

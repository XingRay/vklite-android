//
// Created by leixing on 2025/5/1.
//

#include "SwapchainBuilder.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    SwapchainBuilder::SwapchainBuilder() = default;

    SwapchainBuilder::~SwapchainBuilder() = default;

    std::unique_ptr<Swapchain> SwapchainBuilder::build(const PhysicalDevice &physicalDevice,const Device &device, const Surface &surface, std::vector<uint32_t> queueFamilyIndices) {

        vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getCapabilities(surface);
        vk::Extent2D currentExtent = surfaceCapabilities.currentExtent;

        vk::Extent2D displaySize = VulkanUtil::chooseSwapExtent(surfaceCapabilities, currentExtent.width, currentExtent.height);
        vk::SurfaceFormatKHR imageFormat = VulkanUtil::chooseSwapSurfaceFormat(physicalDevice.getFormats(surface));
        vk::PresentModeKHR presentMode = VulkanUtil::choosePresentMode(physicalDevice.getPresentModes(surface));

        return std::make_unique<Swapchain>(device, surface, surfaceCapabilities, displaySize, imageFormat, presentMode, queueFamilyIndices);
    }

} // vklite
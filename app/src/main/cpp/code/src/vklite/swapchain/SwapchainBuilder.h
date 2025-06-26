//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>

#include "vklite/swapchain/Swapchain.h"
#include "vklite/device/Device.h"
#include "vklite/surface/Surface.h"

namespace vklite {

    class SwapchainBuilder {
    private:
        vk::Device mDevice;
        vk::SwapchainCreateInfoKHR mSwapchainCreateInfo;

        std::vector<uint32_t> mQueueFamilyIndices;

    public:
        SwapchainBuilder();

        ~SwapchainBuilder();

        SwapchainBuilder &device(vk::Device device);

        SwapchainBuilder &surface(vk::SurfaceKHR surface);

        SwapchainBuilder &queueFamilyIndices(std::vector<uint32_t> &&queueFamilyIndices);

        SwapchainBuilder &imageSharingMode(vk::SharingMode sharingMode);

        SwapchainBuilder &minImageCount(uint32_t minImageCount);

        SwapchainBuilder &imageUsage(vk::ImageUsageFlags imageUsage);

        SwapchainBuilder &imageExtent(vk::Extent2D imageExtent);

        SwapchainBuilder &preTransform(vk::SurfaceTransformFlagBitsKHR preTransform);

        SwapchainBuilder &imageFormat(vk::Format imageFormat);

        SwapchainBuilder &imageColorSpace(vk::ColorSpaceKHR imageColorSpace);

        SwapchainBuilder &compositeAlpha(vk::CompositeAlphaFlagBitsKHR compositeAlpha);

        SwapchainBuilder &presentMode(vk::PresentModeKHR presentMode);

        SwapchainBuilder &imageArrayLayers(uint32_t imageArrayLayers);

        SwapchainBuilder &clipped(vk::Bool32 clipped);

        SwapchainBuilder &oldSwapchain(vk::SwapchainKHR oldSwapchain);

        // imageExtent preTransform minImageCount
        SwapchainBuilder &surfaceCapabilities(vk::SurfaceCapabilitiesKHR surfaceCapabilities);

        // imageFormat imageColorSpace
        SwapchainBuilder &surfaceFormat(vk::SurfaceFormatKHR surfaceFormat);

        SwapchainBuilder &chooseSurfaceFormat(const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface, vk::SurfaceFormatKHR preferSurfaceFormat);

        // imageExtent preTransform minImageCount
        // imageFormat imageColorSpace
        SwapchainBuilder &config(const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface);

        SwapchainBuilder &selectPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes,
                                            vk::PresentModeKHR prefer, vk::PresentModeKHR insurance);

        Swapchain build();

        std::unique_ptr<Swapchain> buildUnique();


        //static methods
    private://static

        static vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capability, uint32_t width, uint32_t height);


        // chooseSwapSurfaceFormat
        static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats,
                                                        const std::vector<vk::SurfaceFormatKHR> &preferFormats);

        static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats,
                                                        const vk::SurfaceFormatKHR &preferFormat);

        static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);


        // choosePresentMode
        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes,
                                                    vk::PresentModeKHR prefer, vk::PresentModeKHR insurance);

        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);

    };

} // vklite

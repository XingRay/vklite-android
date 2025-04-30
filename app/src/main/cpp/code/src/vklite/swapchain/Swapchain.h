//
// Created by leixing on 2024/12/24.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/surface/Surface.h"

namespace vklite {
    class Swapchain {
    private://fields
        const Device &mDevice;

        vk::SwapchainKHR mSwapChain;

        vk::Extent2D mDisplaySize;
        vk::SurfaceFormatKHR mSwapChainImageFormat;

        // array of frame buffers and views
        std::vector<vk::Image> mDisplayImages;
        std::vector<vk::ImageView> mDisplayImageViews;

    public://methods
        Swapchain(const Device &device, const Surface &vulkanSurface, uint32_t width, uint32_t height);

        ~Swapchain();

        [[nodiscard]]
        const vk::SwapchainKHR &getSwapChain() const;

        [[nodiscard]]
        vk::Format getDisplayFormat() const;

        [[nodiscard]]
        vk::SurfaceFormatKHR getSurfaceFormat() const;

        [[nodiscard]]
        uint32_t getImageCount() const;

        [[nodiscard]]
        vk::Extent2D getDisplaySize() const;

        [[nodiscard]]
        const std::vector<vk::Image> &getDisplayImages() const;

        [[nodiscard]]
        const std::vector<vk::ImageView> &getDisplayImageViews() const;

    private:

        static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);

        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);

        static vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capability, uint32_t width, uint32_t height);
    };
}

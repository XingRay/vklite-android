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
        Swapchain(const Device &device, const Surface &surface,
                  const vk::SurfaceCapabilitiesKHR &surfaceCapabilities,
                  vk::Extent2D displaySize,
                  vk::SurfaceFormatKHR imageFormat,
                  vk::PresentModeKHR presentMode);

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
    };
}

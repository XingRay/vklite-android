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
        vk::Extent2D mDisplaySize;
        vk::SurfaceFormatKHR mSwapChainImageFormat;

        vk::SwapchainKHR mSwapChain;
        std::vector<vk::Image> mDisplayImages;

    public:
        Swapchain(const Device &device, const Surface &surface,
                  const vk::SurfaceCapabilitiesKHR &surfaceCapabilities,
                  vk::Extent2D displaySize,
                  vk::SurfaceFormatKHR imageFormat,
                  vk::PresentModeKHR presentMode,
                  std::vector<uint32_t> queueFamilyIndices);

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

        vk::ResultValue<uint32_t> acquireNextImage(const vk::Semaphore &semaphore, uint64_t timeout);

        vk::ResultValue<uint32_t> acquireNextImage(const vk::Semaphore &semaphore);
    };
}

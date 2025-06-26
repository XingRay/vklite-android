//
// Created by leixing on 2024/12/24.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "SwapchainMeta.h"
#include "vklite/image_view/ImageView.h"

namespace vklite {

    class Swapchain {
    private://fields
        vk::Device mDevice;
        vk::SwapchainKHR mSwapChain;
        SwapchainMeta mMeta;

    public:

        explicit Swapchain(vk::Device device, vk::SwapchainKHR swapChain, SwapchainMeta meta);

        ~Swapchain();

        Swapchain(const Swapchain &other) = delete;

        Swapchain &operator=(const Swapchain &other) = delete;

        Swapchain(Swapchain &&other) noexcept;

        Swapchain &operator=(Swapchain &&other) noexcept;


        [[nodiscard]]
        const vk::SwapchainKHR &getVkSwapChain() const;

        [[nodiscard]]
        vk::Format getVkFormat() const;

        [[nodiscard]]
        vk::SurfaceFormatKHR getVkSurfaceFormat() const;

        [[nodiscard]]
        uint32_t getImageCount() const;

        [[nodiscard]]
        vk::Extent2D getDisplaySize() const;

        [[nodiscard]]
        std::vector<vk::Image> getDisplayImages() const;

        [[nodiscard]]
        std::vector<ImageView> createDisplayImageViews() const;

        vk::ResultValue<uint32_t> acquireNextImage(const vk::Semaphore &semaphore, uint64_t timeout);

        vk::ResultValue<uint32_t> acquireNextImage(const vk::Semaphore &semaphore);

        std::vector<vk::Viewport> fullScreenViewports();

        std::vector<vk::Rect2D> fullScreenScissors();

        std::vector<vk::Viewport> centerSquareViewports();

        std::vector<vk::Rect2D> centerSquareScissors();
    };
}

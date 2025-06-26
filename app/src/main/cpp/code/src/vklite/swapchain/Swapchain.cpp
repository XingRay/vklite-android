//
// Created by leixing on 2024/12/24.
//

#include "Swapchain.h"

#include <utility>

#include "vklite/Log.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/image_view/ImageViewBuilder.h"

namespace vklite {
    Swapchain::Swapchain(vk::Device device, vk::SwapchainKHR swapChain, SwapchainMeta meta)
            : mDevice(device), mSwapChain(swapChain), mMeta(meta) {}

    Swapchain::~Swapchain() {
        if (mDevice != nullptr && mSwapChain != nullptr) {
            mDevice.destroy(mSwapChain);
            mDevice = nullptr;
            mSwapChain = nullptr;
        }
    }

    Swapchain::Swapchain(Swapchain &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mSwapChain(std::exchange(other.mSwapChain, nullptr)),
              mMeta(other.mMeta) {}

    Swapchain &Swapchain::operator=(Swapchain &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mSwapChain = std::exchange(other.mSwapChain, nullptr);
            mMeta = other.mMeta;
        }
        return *this;
    }

    const vk::SwapchainKHR &Swapchain::getVkSwapChain() const {
        return mSwapChain;
    }

    vk::Format Swapchain::getVkFormat() const {
        return mMeta.surfaceFormat.format;
    }

    vk::SurfaceFormatKHR Swapchain::getVkSurfaceFormat() const {
        return mMeta.surfaceFormat;
    }

    uint32_t Swapchain::getImageCount() const {
        return mMeta.imageCount;
    }

    vk::Extent2D Swapchain::getDisplaySize() const {
        return mMeta.displaySize;
    }

    std::vector<vk::Image> Swapchain::getDisplayImages() const {
        return mDevice.getSwapchainImagesKHR(mSwapChain);
    }

    std::vector<ImageView> Swapchain::createDisplayImageViews() const {
        return ImageViewBuilder().asColorAttachment()
                .device(mDevice)
                .format(getVkFormat())
                .build(getDisplayImages());
    }

    vk::ResultValue<uint32_t> Swapchain::acquireNextImage(const vk::Semaphore &semaphore, uint64_t timeout) {
        return mDevice.acquireNextImageKHR(mSwapChain, timeout, semaphore);
    }

    vk::ResultValue<uint32_t> Swapchain::acquireNextImage(const vk::Semaphore &semaphore) {
        return acquireNextImage(semaphore, std::numeric_limits<uint64_t>::max());
    }

    std::vector<vk::Viewport> Swapchain::fullScreenViewports() {
        std::vector<vk::Viewport> viewports;
        vk::Viewport viewport;
        viewport
                .setX(0.0f)
                .setY(0.0f)
                .setWidth((float) getDisplaySize().width)
                .setHeight((float) getDisplaySize().height)
                .setMinDepth(0.0f)
                .setMaxDepth(1.0f);
        viewports.push_back(viewport);
        return viewports;
    }

    std::vector<vk::Rect2D> Swapchain::fullScreenScissors() {
        std::vector<vk::Rect2D> scissors;
        vk::Rect2D scissor{};
        scissor
                .setOffset(vk::Offset2D{0, 0})
                .setExtent(getDisplaySize());
        scissors.push_back(scissor);
        return scissors;
    }

    std::vector<vk::Viewport> Swapchain::centerSquareViewports() {
        std::vector<vk::Viewport> viewports;

        vk::Viewport viewport;
        viewport.setMinDepth(0.0f)
                .setMaxDepth(1.0f);

        vk::Extent2D displaySize = getDisplaySize();
        if (displaySize.width < displaySize.height) {
            viewport
                    .setX(0.0f)
                    .setY(((float) (displaySize.height - displaySize.width)) * 0.5f)
                    .setWidth((float) getDisplaySize().width)
                    .setHeight((float) getDisplaySize().width);
        } else {
            viewport
                    .setX(((float) (displaySize.width - displaySize.height)) * 0.5f)
                    .setY(0.0f)
                    .setWidth((float) getDisplaySize().height)
                    .setHeight((float) getDisplaySize().height);
        }

        viewports.push_back(viewport);
        return viewports;
    }

    std::vector<vk::Rect2D> Swapchain::centerSquareScissors() {
        std::vector<vk::Rect2D> scissors;
        vk::Rect2D scissor{};

        vk::Extent2D displaySize = getDisplaySize();
        if (displaySize.width < displaySize.height) {
            scissor
                    .setOffset(vk::Offset2D{0, (int32_t) (displaySize.height - displaySize.width) / 2})
                    .setExtent({getDisplaySize().width, getDisplaySize().width});
        } else {
            scissor
                    .setOffset(vk::Offset2D{(int32_t) (displaySize.width - displaySize.height) / 2, 0})
                    .setExtent({getDisplaySize().width, getDisplaySize().width});
        }

        scissors.push_back(scissor);
        return scissors;
    }
}

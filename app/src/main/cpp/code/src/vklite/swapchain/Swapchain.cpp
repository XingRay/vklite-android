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

    const vk::SwapchainKHR &Swapchain::getSwapChain() const {
        return mSwapChain;
    }

    vk::Format Swapchain::getDisplayFormat() const {
        return mMeta.surfaceFormat.format;
    }

    vk::SurfaceFormatKHR Swapchain::getSurfaceFormat() const {
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
        return ImageViewBuilder::colorImageViewBuilder()
                .device(mDevice)
                .format(getDisplayFormat())
                .build(getDisplayImages());
    }

    vk::ResultValue<uint32_t> Swapchain::acquireNextImage(const vk::Semaphore &semaphore, uint64_t timeout) {
        return mDevice.acquireNextImageKHR(mSwapChain, timeout, semaphore);
    }

    vk::ResultValue<uint32_t> Swapchain::acquireNextImage(const vk::Semaphore &semaphore) {
        return acquireNextImage(semaphore, std::numeric_limits<uint64_t>::max());
    }
}

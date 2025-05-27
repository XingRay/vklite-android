//
// Created by leixing on 2024/12/24.
//

#include "Swapchain.h"
#include "vklite/Log.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {
    Swapchain::Swapchain(const Device &device,
                         const Surface &surface,
                         const vk::SurfaceCapabilitiesKHR &surfaceCapabilities,
                         vk::Extent2D displaySize,
                         vk::SurfaceFormatKHR imageFormat,
                         vk::PresentModeKHR presentMode,
                         std::vector<uint32_t> queueFamilyIndices)
            : mDevice(device), mDisplaySize(displaySize), mSwapChainImageFormat(imageFormat) {
        const vk::Device &vkDevice = device.getDevice();

        LOG_D("capabilities.minImageCount:%d, maxImageCount:%d", surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);
        uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
        // capabilities.maxImageCount == 0 表示不做限制
        if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
            imageCount = surfaceCapabilities.maxImageCount;
        }
        LOG_D("imageCount: %d", imageCount);

//        std::vector<uint32_t> queueFamilyIndices = device.getQueueFamilyIndices();
        vk::SharingMode sharingMode;
//        if (queueFamilyIndices.size() == 1) {
        sharingMode = vk::SharingMode::eExclusive;
//        } else {
//            sharingMode = vk::SharingMode::eConcurrent;
//        }

        vk::SwapchainCreateInfoKHR swapchainCreateInfo = vk::SwapchainCreateInfoKHR{}
                .setSurface(surface.getSurface())
                .setMinImageCount(imageCount)
                .setImageFormat(mSwapChainImageFormat.format)
                .setImageColorSpace(mSwapChainImageFormat.colorSpace)
                .setImageExtent(mDisplaySize)
                .setImageArrayLayers(1)
                .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
//                .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment| vk::ImageUsageFlagBits::eTransferDst)
                .setImageSharingMode(sharingMode)
                .setQueueFamilyIndices(queueFamilyIndices)
                .setPreTransform(surfaceCapabilities.currentTransform)
                .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
//                .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eInherit)
                .setPresentMode(presentMode)
                .setClipped(vk::True)
                .setOldSwapchain(nullptr);

        mSwapChain = vkDevice.createSwapchainKHR(swapchainCreateInfo);
        mDisplayImages = vkDevice.getSwapchainImagesKHR(mSwapChain);
    }

    Swapchain::~Swapchain() {
        LOG_D("Swapchain::~Swapchain()");
        const vk::Device &vkDevice = mDevice.getDevice();
        // 通过 getSwapchainImagesKHR 获取的对象, 不是create的对象,不需要destroy, swapchain 会自动销毁
        vkDevice.destroy(mSwapChain);
    }

    const vk::SwapchainKHR &Swapchain::getSwapChain() const {
        return mSwapChain;
    }

    vk::Format Swapchain::getDisplayFormat() const {
        return mSwapChainImageFormat.format;
    }

    vk::SurfaceFormatKHR Swapchain::getSurfaceFormat() const {
        return mSwapChainImageFormat;
    }

    uint32_t Swapchain::getImageCount() const {
        return mDisplayImages.size();
    }

    vk::Extent2D Swapchain::getDisplaySize() const {
        return mDisplaySize;
    }

    const std::vector<vk::Image> &Swapchain::getDisplayImages() const {
        return mDisplayImages;
    }

    vk::ResultValue<uint32_t> Swapchain::acquireNextImage(const vk::Semaphore &semaphore, uint64_t timeout) {
        return mDevice.getDevice().acquireNextImageKHR(mSwapChain, timeout, semaphore);
    }

    vk::ResultValue<uint32_t> Swapchain::acquireNextImage(const vk::Semaphore &semaphore) {
        return acquireNextImage(semaphore, std::numeric_limits<uint64_t>::max());
    }
}

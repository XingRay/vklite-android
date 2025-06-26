//
// Created by leixing on 2025/5/1.
//

#include "SwapchainBuilder.h"

#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

#include "SwapchainMeta.h"

namespace vklite {

    SwapchainBuilder::SwapchainBuilder() {
        mSwapchainCreateInfo = vk::SwapchainCreateInfoKHR{}
//                .setSurface(mSurface)
//                .setMinImageCount(imageCount)
//                .setImageFormat(imageFormat.format)
//                .setImageColorSpace(imageFormat.colorSpace)
//                .setImageExtent(displaySize)
                .setImageArrayLayers(1)
                .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
//                .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment| vk::ImageUsageFlagBits::eTransferDst)
//                .setImageSharingMode(sharingMode)
                .setQueueFamilyIndices(mQueueFamilyIndices)
//                .setPreTransform(surfaceCapabilities.currentTransform)
                .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
//                .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eInherit)
//                .setPresentMode(presentMode)
                .setClipped(vk::True)
                .setOldSwapchain(nullptr);
    }

    SwapchainBuilder::~SwapchainBuilder() = default;

    SwapchainBuilder &SwapchainBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::surface(vk::SurfaceKHR surface) {
        mSwapchainCreateInfo.setSurface(surface);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::queueFamilyIndices(std::vector<uint32_t> &&queueFamilyIndices) {
        mQueueFamilyIndices = std::move(queueFamilyIndices);
        mSwapchainCreateInfo.setQueueFamilyIndices(mQueueFamilyIndices);

        vk::SharingMode sharingMode;
        if (mQueueFamilyIndices.size() == 1) {
            sharingMode = vk::SharingMode::eExclusive;
        } else {
            sharingMode = vk::SharingMode::eConcurrent;
        }
        mSwapchainCreateInfo.setImageSharingMode(sharingMode);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::imageSharingMode(vk::SharingMode sharingMode) {
        mSwapchainCreateInfo.setImageSharingMode(sharingMode);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::minImageCount(uint32_t minImageCount) {
        mSwapchainCreateInfo.setMinImageCount(minImageCount);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::imageUsage(vk::ImageUsageFlags imageUsage) {
        mSwapchainCreateInfo.setImageUsage(imageUsage);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::imageExtent(vk::Extent2D imageExtent) {
        mSwapchainCreateInfo.setImageExtent(imageExtent);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::preTransform(vk::SurfaceTransformFlagBitsKHR preTransform) {
        mSwapchainCreateInfo.setPreTransform(preTransform);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::imageFormat(vk::Format imageFormat) {
        mSwapchainCreateInfo.setImageFormat(imageFormat);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::imageColorSpace(vk::ColorSpaceKHR imageColorSpace) {
        mSwapchainCreateInfo.setImageColorSpace(imageColorSpace);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::compositeAlpha(vk::CompositeAlphaFlagBitsKHR compositeAlpha) {
        mSwapchainCreateInfo.setCompositeAlpha(compositeAlpha);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::presentMode(vk::PresentModeKHR presentMode) {
        mSwapchainCreateInfo.setPresentMode(presentMode);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::imageArrayLayers(uint32_t imageArrayLayers) {
        mSwapchainCreateInfo.setImageArrayLayers(imageArrayLayers);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::clipped(vk::Bool32 clipped) {
        mSwapchainCreateInfo.setClipped(clipped);
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::oldSwapchain(vk::SwapchainKHR oldSwapchain) {
        mSwapchainCreateInfo.setOldSwapchain(oldSwapchain);
        return *this;
    }

    // imageExtent preTransform minImageCount
    SwapchainBuilder &SwapchainBuilder::surfaceCapabilities(vk::SurfaceCapabilitiesKHR surfaceCapabilities) {
        imageExtent(surfaceCapabilities.currentExtent);
        preTransform(surfaceCapabilities.currentTransform);

        uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
        // capabilities.maxImageCount == 0 表示不做限制
        if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
            imageCount = surfaceCapabilities.maxImageCount;
        }
        LOG_D("imageCount: %d", imageCount);
        minImageCount(imageCount);

        return *this;
    }

    // imageFormat imageColorSpace
    SwapchainBuilder &SwapchainBuilder::surfaceFormat(vk::SurfaceFormatKHR surfaceFormat) {
        imageFormat(surfaceFormat.format);
        imageColorSpace(surfaceFormat.colorSpace);
        return *this;
    }

    // imageFormat imageColorSpace
    SwapchainBuilder &SwapchainBuilder::chooseSurfaceFormat(const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface, vk::SurfaceFormatKHR preferSurfaceFormat) {
        surfaceFormat(chooseSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(surface), preferSurfaceFormat));
        return *this;
    }

    // imageExtent preTransform minImageCount imageFormat imageColorSpace presentMode
    SwapchainBuilder &SwapchainBuilder::config(const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface) {
        surfaceCapabilities(physicalDevice.getSurfaceCapabilitiesKHR(surface));
        surfaceFormat(chooseSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(surface)));
        presentMode(choosePresentMode(physicalDevice.getSurfacePresentModesKHR(surface)));
        return *this;
    }

    SwapchainBuilder &SwapchainBuilder::selectPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes,
                                                          vk::PresentModeKHR prefer, vk::PresentModeKHR insurance) {
        presentMode(choosePresentMode(availablePresentModes, prefer, insurance));
        return *this;
    }

    Swapchain SwapchainBuilder::build() {
        if (mDevice == nullptr) {
            LOG_E("mDevice== nullptr, must invoke SwapchainBuilder::device(vk::Device device)");
            throw std::runtime_error("SwapchainBuilder::build(): mDevice== nullptr, must invoke SwapchainBuilder::device(vk::Device device)");
        }
        if (mSwapchainCreateInfo.surface == nullptr) {
            LOG_E("mSwapchainCreateInfo.surface== nullptr, must invoke SwapchainBuilder::surface(vk::SurfaceKHR surface)");
            throw std::runtime_error("SwapchainBuilder::build(): mSwapchainCreateInfo.surface== nullptr, must invoke SwapchainBuilder::surface(vk::SurfaceKHR surface)");
        }

        if (mQueueFamilyIndices.empty()) {
            LOG_E("mQueueFamilyIndices not set , mQueueFamilyIndices.size() == 0");
            throw std::runtime_error("SwapchainBuilder::build(): mQueueFamilyIndices not set , mQueueFamilyIndices.size() == 0");
        } else if (mQueueFamilyIndices.size() == 1) {
            if (mSwapchainCreateInfo.imageSharingMode != vk::SharingMode::eExclusive) {
                LOG_E("mSwapchainCreateInfo.imageSharingMode should be vk::SharingMode::eExclusive when mQueueFamilyIndices.size() == 1 , but mSwapchainCreateInfo.imageSharingMode:%d",
                      mSwapchainCreateInfo.imageSharingMode);
                throw std::runtime_error("SwapchainBuilder::build():  mSwapchainCreateInfo.imageSharingMode error");
            }
        } else {
            if (mSwapchainCreateInfo.imageSharingMode != vk::SharingMode::eConcurrent) {
                LOG_E("mSwapchainCreateInfo.imageSharingMode should be vk::SharingMode::eConcurrent when mQueueFamilyIndices.size() > 1 , but mSwapchainCreateInfo.imageSharingMode:%d",
                      mSwapchainCreateInfo.imageSharingMode);
                throw std::runtime_error("SwapchainBuilder::build():  mSwapchainCreateInfo.imageSharingMode error");
            }
        }

        vk::SwapchainKHR swapChain = mDevice.createSwapchainKHR(mSwapchainCreateInfo);

        uint32_t imageCount = mDevice.getSwapchainImagesKHR(swapChain).size();

        SwapchainMeta meta = {{mSwapchainCreateInfo.imageFormat, mSwapchainCreateInfo.imageColorSpace}, imageCount, mSwapchainCreateInfo.imageExtent};

        return Swapchain(mDevice, swapChain, meta);
    }

    std::unique_ptr<Swapchain> SwapchainBuilder::buildUnique() {
        return std::make_unique<Swapchain>(build());
    }

    vk::Extent2D SwapchainBuilder::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capability, uint32_t width, uint32_t height) {
        if (capability.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capability.currentExtent;
        }

        return vk::Extent2D{
                std::clamp(width, capability.minImageExtent.width, capability.maxImageExtent.width),
                std::clamp(height, capability.minImageExtent.height, capability.maxImageExtent.height),
        };
    }

    vk::SurfaceFormatKHR SwapchainBuilder::chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats,
                                                               const std::vector<vk::SurfaceFormatKHR> &preferFormats) {

        for (const auto &availableFormat: availableFormats) {
            for (const vk::SurfaceFormatKHR &preferFormat: preferFormats) {
                if (availableFormat.format == preferFormat.format && availableFormat.colorSpace == preferFormat.colorSpace) {
                    return availableFormat;
                }
            }
        }
        LOG_E("none of preferFormats found in availableFormats, return availableFormats[0]");
        return availableFormats[0];
    }

    vk::SurfaceFormatKHR SwapchainBuilder::chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats,
                                                               const vk::SurfaceFormatKHR &preferFormat) {

        for (const auto &availableFormat: availableFormats) {
            if (availableFormat.format == preferFormat.format && availableFormat.colorSpace == preferFormat.colorSpace) {
                return availableFormat;
            }
        }
        LOG_E("preferFormat not found in availableFormats, return availableFormats[0]");
        return availableFormats[0];
    }

    vk::SurfaceFormatKHR SwapchainBuilder::chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
        vk::SurfaceFormatKHR preferFormat = {vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear};
//        vk::SurfaceFormatKHR preferFormat = {vk::Format::eR8G8B8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};

        return chooseSurfaceFormat(availableFormats, preferFormat);
    }


    vk::PresentModeKHR SwapchainBuilder::choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes,
                                                           vk::PresentModeKHR prefer, vk::PresentModeKHR insurance) {
        bool insuranceFound = false;
        for (const auto &availablePresentMode: availablePresentModes) {
            if (availablePresentMode == prefer) {
                return prefer;
            }
            if (availablePresentMode == insurance) {
                insuranceFound = true;
            }
        }

        if (insuranceFound) {
            return insurance;
        } else {
            LOG_E("prefer:%d and insurance:%d not found in availablePresentModes, return availablePresentModes[0]:%d",
                  prefer, insurance, availablePresentModes[0]);
            return availablePresentModes[0];
        }
    }

    vk::PresentModeKHR SwapchainBuilder::choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
        return choosePresentMode(availablePresentModes, vk::PresentModeKHR::eMailbox, vk::PresentModeKHR::eFifo);
    }

} // vklite
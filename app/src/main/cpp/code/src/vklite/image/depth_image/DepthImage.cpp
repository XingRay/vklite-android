//
// Created by leixing on 2025/5/19.
//

#include "DepthImage.h"

namespace vklite {

    DepthImage::DepthImage(Image &&image)
            : mImage(std::move(image)) {}

    DepthImage::~DepthImage() = default;

    DepthImage::DepthImage(DepthImage &&other) noexcept
            : mImage(std::move(other.mImage)) {}

    const vk::Image &DepthImage::getImage() const {
        return mImage.getImage();
    }

    void DepthImage::transitionImageLayout(const CommandPool &commandPool,
                                           vk::ImageLayout oldImageLayout,
                                           vk::ImageLayout newImageLayout,
                                           uint32_t levelCount,
                                           uint32_t srcQueueFamilyIndex,
                                           uint32_t dstQueueFamilyIndex,
                                           vk::ImageAspectFlags imageAspectFlags) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandTransitionImageLayout(commandBuffer, oldImageLayout, newImageLayout, levelCount, srcQueueFamilyIndex, dstQueueFamilyIndex, imageAspectFlags);
        });
    }

    void DepthImage::recordCommandTransitionImageLayout(const vk::CommandBuffer &commandBuffer,
                                                        vk::ImageLayout oldImageLayout,
                                                        vk::ImageLayout newImageLayout,
                                                        uint32_t levelCount,
                                                        uint32_t srcQueueFamilyIndex,
                                                        uint32_t dstQueueFamilyIndex,
                                                        vk::ImageAspectFlags imageAspectFlags) {
        mImage.recordTransitionImageLayout(commandBuffer, oldImageLayout, newImageLayout, levelCount, srcQueueFamilyIndex, dstQueueFamilyIndex, imageAspectFlags);
    }

    void DepthImage::transitionImageLayout(const CommandPool &commandPool) {
        transitionImageLayout(commandPool, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1,
                              vk::QueueFamilyIgnored, vk::QueueFamilyIgnored, vk::ImageAspectFlagBits::eDepth);
    }

    void DepthImage::recordCommandTransitionImageLayout(const vk::CommandBuffer &commandBuffer) {
        recordCommandTransitionImageLayout(commandBuffer, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1,
                                           vk::QueueFamilyIgnored, vk::QueueFamilyIgnored, vk::ImageAspectFlagBits::eDepth);
    }

} // vklite
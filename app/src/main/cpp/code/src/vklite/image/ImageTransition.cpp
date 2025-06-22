//
// Created by leixing on 2025/5/18.
//

#include "ImageTransition.h"

namespace vklite {

    ImageTransition::ImageTransition(vk::ImageLayout oldImageLayout,
                                     vk::ImageLayout newImageLayout,
                                     uint32_t srcQueueFamilyIndex,
                                     uint32_t dstQueueFamilyIndex,
                                     uint32_t levelCount,
                                     vk::ImageAspectFlags aspectMask)
            : mOldImageLayout(oldImageLayout),
              mNewImageLayout(newImageLayout),
              mSrcQueueFamilyIndex(srcQueueFamilyIndex),
              mDstQueueFamilyIndex(dstQueueFamilyIndex),
              mLevelCount(levelCount),
              mAspectMask(aspectMask) {}

    ImageTransition::~ImageTransition() = default;

    vk::ImageLayout ImageTransition::getOldImageLayout() const {
        return mOldImageLayout;
    }

    vk::ImageLayout ImageTransition::getNewImageLayout() const {
        return mNewImageLayout;
    }

    uint32_t ImageTransition::getLevelCount() const {
        return mLevelCount;
    }

    uint32_t ImageTransition::getSrcQueueFamilyIndex() const {
        return mSrcQueueFamilyIndex;
    }

    uint32_t ImageTransition::getDstQueueFamilyIndex() const {
        return mDstQueueFamilyIndex;
    }

    vk::ImageAspectFlags ImageTransition::getAspectMask() const {
        return mAspectMask;
    }

    ImageTransition ImageTransition::depthImageTransition() {
        return ImageTransition{vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::QueueFamilyIgnored, vk::QueueFamilyIgnored, 1, vk::ImageAspectFlagBits::eDepth};
    }

    ImageTransition ImageTransition::textureImageTransition() {
        return ImageTransition{vk::ImageLayout::eUndefined, vk::ImageLayout::eUndefined, vk::QueueFamilyIgnored, vk::QueueFamilyIgnored, 1, vk::ImageAspectFlagBits::eColor};
    }

} // vklite
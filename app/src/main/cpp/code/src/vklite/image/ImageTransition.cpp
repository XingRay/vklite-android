//
// Created by leixing on 2025/5/18.
//

#include "ImageTransition.h"

namespace vklite {

    ImageTransition::ImageTransition(vk::ImageLayout oldImageLayout,
                                     vk::ImageLayout newImageLayout,
                                     uint32_t levelCount,
                                     uint32_t srcQueueFamilyIndex,
                                     uint32_t dstQueueFamilyIndex,
                                     vk::ImageAspectFlags aspectMask)
            : mOldImageLayout(oldImageLayout),
              mNewImageLayout(newImageLayout),
              mLevelCount(levelCount),
              mSrcQueueFamilyIndex(srcQueueFamilyIndex),
              mDstQueueFamilyIndex(dstQueueFamilyIndex),
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
        return ImageTransition{vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1, vk::QueueFamilyIgnored, vk::QueueFamilyIgnored, vk::ImageAspectFlagBits::eDepth};
    }

    ImageTransition ImageTransition::textureImageTransition() {
        return ImageTransition{vk::ImageLayout::eUndefined, vk::ImageLayout::eUndefined, 1, vk::QueueFamilyIgnored, vk::QueueFamilyIgnored, vk::ImageAspectFlagBits::eColor};
    }

} // vklite
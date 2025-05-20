//
// Created by leixing on 2025/5/18.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class ImageTransition {
    private:
        vk::ImageLayout mOldImageLayout;
        vk::ImageLayout mNewImageLayout;
        uint32_t mLevelCount;
        uint32_t mSrcQueueFamilyIndex;
        uint32_t mDstQueueFamilyIndex;
        vk::ImageAspectFlags mAspectMask;

    public:
        ImageTransition(vk::ImageLayout oldImageLayout,
                        vk::ImageLayout newImageLayout,
                        uint32_t levelCount,
                        uint32_t srcQueueFamilyIndex,
                        uint32_t dstQueueFamilyIndex,
                        vk::ImageAspectFlags aspectMask);

        ~ImageTransition();

        [[nodiscard]]
        vk::ImageLayout getOldImageLayout() const;

        [[nodiscard]]
        vk::ImageLayout getNewImageLayout()const;

        [[nodiscard]]
        uint32_t getLevelCount()const;

        [[nodiscard]]
        uint32_t getSrcQueueFamilyIndex()const;

        [[nodiscard]]
        uint32_t getDstQueueFamilyIndex()const;

        [[nodiscard]]
        vk::ImageAspectFlags getAspectMask()const;

    public://static

        static ImageTransition depthImageTransition();

        static ImageTransition textureImageTransition();
    };

} // vklite

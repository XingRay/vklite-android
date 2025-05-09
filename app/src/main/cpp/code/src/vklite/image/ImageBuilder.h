//
// Created by leixing on 2025/4/11.
//

#pragma once

#include "ImageBuilderInterface.h"

namespace vklite {

    class ImageBuilder {
    private:
        uint32_t mWidth;
        uint32_t mHeight;
        uint32_t mMipLevels;
        vk::Format mFormat;
        vk::ImageUsageFlags mImageUsageFlags;
        vk::SampleCountFlagBits mSampleCountFlagBits;
        vk::ImageTiling mImageTiling;

        // TransitionImageLayout params
        vk::ImageLayout mOldImageLayout;
        vk::ImageLayout mNewImageLayout;
        uint32_t mSrcQueueFamilyIndex;
        uint32_t mDstQueueFamilyIndex;
//        vk::ImageAspectFlags mImageAspectFlags;

    public:
        ImageBuilder();

        ~ImageBuilder();

        ImageBuilder &width(uint32_t width);

        ImageBuilder &height(uint32_t height);

        ImageBuilder &mipLevels(uint32_t mipLevels);

        ImageBuilder &format(vk::Format format);

        ImageBuilder &imageUsageFlags(vk::ImageUsageFlags imageUsageFlags);

        ImageBuilder &sampleCountFlagBits(vk::SampleCountFlagBits sampleCountFlagBits);

        ImageBuilder &imageTiling(vk::ImageTiling imageTiling);

        // TransitionImageLayout params
        ImageBuilder &oldImageLayout(vk::ImageLayout oldImageLayout);

        ImageBuilder &newImageLayout(vk::ImageLayout newImageLayout);

        ImageBuilder &srcQueueFamilyIndex(uint32_t srcQueueFamilyIndex);

        ImageBuilder &dstQueueFamilyIndex(uint32_t dstQueueFamilyIndex);

//        ImageBuilder &imageAspectFlags(vk::ImageAspectFlags imageAspectFlags);

        [[nodiscard]]
        std::unique_ptr<ImageInterface> build(const Device &device);

    public:// static
        static ImageBuilder colorImageBuilder();

        static ImageBuilder depthImageBuilder();
    };

} // vklite

//
// Created by leixing on 2025/4/11.
//

#include "ImageBuilder.h"
#include "Image.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    ImageBuilder::ImageBuilder()
            : mImageTiling(vk::ImageTiling::eOptimal), mSampleCountFlagBits(vk::SampleCountFlagBits::e1), mMipLevels(1),
              mSrcQueueFamilyIndex(vk::QueueFamilyIgnored), mDstQueueFamilyIndex(vk::QueueFamilyIgnored),
              mOldImageLayout(vk::ImageLayout::eUndefined), mNewImageLayout(vk::ImageLayout::eUndefined) {};

    ImageBuilder::~ImageBuilder() = default;

    ImageBuilder &ImageBuilder::width(uint32_t width) {
        mWidth = width;
        return *this;
    }

    ImageBuilder &ImageBuilder::height(uint32_t height) {
        mHeight = height;
        return *this;
    }

    ImageBuilder &ImageBuilder::mipLevels(uint32_t mipLevels) {
        mMipLevels = mipLevels;
        return *this;
    }

    ImageBuilder &ImageBuilder::format(vk::Format format) {
        mFormat = format;
        return *this;
    }

    ImageBuilder &ImageBuilder::imageUsageFlags(vk::ImageUsageFlags imageUsageFlags) {
        mImageUsageFlags = imageUsageFlags;
        return *this;
    }

    ImageBuilder &ImageBuilder::sampleCountFlagBits(vk::SampleCountFlagBits sampleCountFlagBits) {
        mSampleCountFlagBits = sampleCountFlagBits;
        return *this;
    }

    ImageBuilder &ImageBuilder::imageTiling(vk::ImageTiling imageTiling) {
        mImageTiling = imageTiling;
        return *this;
    }

    // TransitionImageLayout params
    ImageBuilder &ImageBuilder::oldImageLayout(vk::ImageLayout oldImageLayout) {
        mOldImageLayout = oldImageLayout;
        return *this;
    }

    ImageBuilder &ImageBuilder::newImageLayout(vk::ImageLayout newImageLayout) {
        mNewImageLayout = newImageLayout;
        return *this;
    }

    ImageBuilder &ImageBuilder::srcQueueFamilyIndex(uint32_t srcQueueFamilyIndex) {
        mSrcQueueFamilyIndex = srcQueueFamilyIndex;
        return *this;
    }

    ImageBuilder &ImageBuilder::dstQueueFamilyIndex(uint32_t dstQueueFamilyIndex) {
        mDstQueueFamilyIndex = dstQueueFamilyIndex;
        return *this;
    }

//    ImageBuilder &ImageBuilder::imageAspectFlags(vk::ImageAspectFlags imageAspectFlags) {
//        mImageAspectFlags = imageAspectFlags;
//        return *this;
//    }

    std::unique_ptr<ImageInterface> ImageBuilder::build(const Device &device) {
        vk::ImageAspectFlags imageAspectFlags = VulkanUtil::calcImageAspectFlags(mNewImageLayout, mFormat);
        return std::make_unique<Image>(device, mWidth, mHeight, mFormat, mMipLevels, mImageUsageFlags, mSampleCountFlagBits, mImageTiling,
                                       mOldImageLayout, mNewImageLayout, mSrcQueueFamilyIndex,
                                       mDstQueueFamilyIndex, imageAspectFlags);
    }

    ImageBuilder ImageBuilder::colorImageBuilder() {
        return ImageBuilder()
                .imageUsageFlags(vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment);

    }

    ImageBuilder ImageBuilder::depthImageBuilder() {
        return ImageBuilder()
                .imageUsageFlags(vk::ImageUsageFlagBits::eDepthStencilAttachment)
//                .oldImageLayout(vk::ImageLayout::eUndefined)
                .newImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
    }

} // vklite
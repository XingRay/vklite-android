//
// Created by leixing on 2025/4/11.
//

#include "ImageBuilder.h"
#include "Image.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    ImageBuilder::ImageBuilder()
            : mWidth(0), mHeight(0), mFormat(vk::Format::eUndefined),
              mImageUsageFlags(vk::ImageUsageFlagBits::eColorAttachment),
              mImageTiling(vk::ImageTiling::eOptimal),
              mSampleCount(vk::SampleCountFlagBits::e1),
              mMipLevels(1) {};

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

    ImageBuilder &ImageBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mSampleCount = sampleCount;
        return *this;
    }

    ImageBuilder &ImageBuilder::imageTiling(vk::ImageTiling imageTiling) {
        mImageTiling = imageTiling;
        return *this;
    }

    std::unique_ptr<Image> ImageBuilder::buildUnique(const Device &device) {
//        vk::ImageAspectFlags imageAspectFlags = VulkanUtil::calcImageAspectFlags(mNewImageLayout, mFormat);
        return std::make_unique<Image>(device, mWidth, mHeight, mFormat, mMipLevels, mImageUsageFlags, mSampleCount, mImageTiling);
    }

    ImageBuilder ImageBuilder::colorImageBuilder() {
        return ImageBuilder()
                .imageUsageFlags(vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment);

    }

    ImageBuilder ImageBuilder::depthImageBuilder() {
        return ImageBuilder()
                .imageUsageFlags(vk::ImageUsageFlagBits::eDepthStencilAttachment);
//                .oldImageLayout(vk::ImageLayout::eUndefined)
//                .newImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
    }

    ImageBuilder ImageBuilder::textureImageBuilder() {
        return ImageBuilder()
                .imageUsageFlags(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
                .sampleCount(vk::SampleCountFlagBits::e1);
    }

} // vklite
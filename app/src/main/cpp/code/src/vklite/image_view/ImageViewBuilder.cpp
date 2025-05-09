//
// Created by leixing on 2025/5/7.
//

#include "ImageViewBuilder.h"

namespace vklite {

    ImageViewBuilder::ImageViewBuilder()
            : mLevelCount(1), mFormat(vk::Format::eUndefined) {}

    ImageViewBuilder::~ImageViewBuilder() = default;

    ImageViewBuilder &ImageViewBuilder::format(vk::Format format) {
        mFormat = format;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::imageAspect(vk::ImageAspectFlags imageAspect) {
        mImageAspect = imageAspect;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::levelCount(uint32_t levelCount) {
        mLevelCount = levelCount;
        return *this;
    }

    std::unique_ptr<ImageView> ImageViewBuilder::build(const Device &device, const vk::Image &image) {
        return std::make_unique<ImageView>(device, image, mFormat, mImageAspect, mLevelCount);
    }

    std::unique_ptr<ImageView> ImageViewBuilder::build(const Device &device, const ImageInterface &image) {
        return std::make_unique<ImageView>(device, image.getImage(), image.getFormat(), mImageAspect, mLevelCount);
    }

    std::vector<ImageView> ImageViewBuilder::build(const Device &device, const std::vector<vk::Image> &images) {
        std::vector<ImageView> imageViews;
        imageViews.reserve(images.size());

        for (const vk::Image &image: images) {
            imageViews.emplace_back(device, image, mFormat, mImageAspect, mLevelCount);
        }

        return imageViews;
    }

    ImageViewBuilder ImageViewBuilder::colorImageViewBuilder() {
        return ImageViewBuilder()
                .imageAspect(vk::ImageAspectFlagBits::eColor);
    }

    ImageViewBuilder ImageViewBuilder::depthImageViewBuilder() {
        return ImageViewBuilder()
                .imageAspect(vk::ImageAspectFlagBits::eDepth);
    }

} // vklite
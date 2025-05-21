//
// Created by leixing on 2025/5/7.
//

#include "ImageViewBuilder.h"

namespace vklite {

    ImageViewBuilder::ImageViewBuilder()
            : mImageViewCreateInfo{} {}

    ImageViewBuilder::~ImageViewBuilder() = default;

    ImageViewBuilder &ImageViewBuilder::aspectMask(vk::ImageAspectFlags aspectMask) {
        mImageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::baseMipLevel(uint32_t baseMipLevel) {
        mImageViewCreateInfo.subresourceRange.baseMipLevel = baseMipLevel;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::levelCount(uint32_t levelCount) {
        mImageViewCreateInfo.subresourceRange.levelCount = levelCount;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::baseArrayLayer(uint32_t baseArrayLayer) {
        mImageViewCreateInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::layerCount(uint32_t layerCount) {
        mImageViewCreateInfo.subresourceRange.layerCount = layerCount;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::componentMappingRed(vk::ComponentSwizzle mapping) {
        mImageViewCreateInfo.components.r = mapping;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::componentMappingGreen(vk::ComponentSwizzle mapping) {
        mImageViewCreateInfo.components.g = mapping;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::componentMappingBlue(vk::ComponentSwizzle mapping) {
        mImageViewCreateInfo.components.b = mapping;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::componentMappingAlpha(vk::ComponentSwizzle mapping) {
        mImageViewCreateInfo.components.a = mapping;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::viewType(vk::ImageViewType viewType) {
        mImageViewCreateInfo.viewType = viewType;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::format(vk::Format format) {
        mImageViewCreateInfo.format = format;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::next(const void *next) {
        mImageViewCreateInfo.pNext = next;
        return *this;
    }

    ImageView ImageViewBuilder::build(const Device &device, const vk::Image &image) {
        mImageViewCreateInfo.image = image;
//        return ImageView(device, mImageViewCreateInfo);
        return {device, mImageViewCreateInfo};
    }

    std::unique_ptr<ImageView> ImageViewBuilder::buildUnique(const Device &device, const vk::Image &image) {
        mImageViewCreateInfo.image = image;
        return std::make_unique<ImageView>(device, mImageViewCreateInfo);
    }

    ImageView ImageViewBuilder::build(const Device &device, const ImageInterface &image) {
        mImageViewCreateInfo.image = image.getImage();
//        return ImageView(device, mImageViewCreateInfo);
        return {device, mImageViewCreateInfo};
    }

    std::unique_ptr<ImageView> ImageViewBuilder::buildUnique(const Device &device, const ImageInterface &image) {
        mImageViewCreateInfo.image = image.getImage();
        return std::make_unique<ImageView>(device, mImageViewCreateInfo);
    }

    std::vector<ImageView> ImageViewBuilder::build(const Device &device, const std::vector<vk::Image> &images) {
        std::vector<ImageView> imageViews;
        imageViews.reserve(images.size());

        for (const vk::Image &image: images) {
            mImageViewCreateInfo.image = image;
            imageViews.emplace_back(device, mImageViewCreateInfo);
        }

        return imageViews;
    }

    std::vector<ImageView> ImageViewBuilder::build(const Device &device, const std::vector<ImageInterface> &images) {
        std::vector<ImageView> imageViews;
        imageViews.reserve(images.size());

        for (const ImageInterface &image: images) {
            mImageViewCreateInfo.image = image.getImage();
            imageViews.emplace_back(device, mImageViewCreateInfo);
        }

        return imageViews;
    }

    ImageViewBuilder ImageViewBuilder::defaultImageViewBuilder() {
        return ImageViewBuilder()
                .baseMipLevel(0)
                .levelCount(1)
                .baseArrayLayer(0)
                .layerCount(1)
                .componentMappingRed(vk::ComponentSwizzle::eIdentity)
                .componentMappingGreen(vk::ComponentSwizzle::eIdentity)
                .componentMappingBlue(vk::ComponentSwizzle::eIdentity)
                .componentMappingAlpha(vk::ComponentSwizzle::eIdentity)
                .viewType(vk::ImageViewType::e2D)
                .format(vk::Format::eUndefined);
    }

    ImageViewBuilder ImageViewBuilder::colorImageViewBuilder() {
        return defaultImageViewBuilder()
                .aspectMask(vk::ImageAspectFlagBits::eColor);
    }

    ImageViewBuilder ImageViewBuilder::depthImageViewBuilder() {
        return defaultImageViewBuilder()
                .aspectMask(vk::ImageAspectFlagBits::eDepth);
    }

} // vklite
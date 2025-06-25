//
// Created by leixing on 2025-06-22.
//

#include "ImageMemoryBarrierBuilder.h"

namespace vklite {

    ImageMemoryBarrierBuilder::ImageMemoryBarrierBuilder()
            : mImageMemoryBarrier{} {
    }

    ImageMemoryBarrierBuilder::~ImageMemoryBarrierBuilder() = default;

    // SubresourceRange
    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::baseMipmapLevel(uint32_t baseMipmapLevel) {
        mImageMemoryBarrier.subresourceRange.setBaseMipLevel(baseMipmapLevel);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::levelCount(uint32_t levelCount) {
        mImageMemoryBarrier.subresourceRange.setLevelCount(levelCount);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::baseArrayLayer(uint32_t baseArrayLayer) {
        mImageMemoryBarrier.subresourceRange.setBaseArrayLayer(baseArrayLayer);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::layerCount(uint32_t layerCount) {
        mImageMemoryBarrier.subresourceRange.setLayerCount(layerCount);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::aspectMask(vk::ImageAspectFlags aspectMask) {
        mImageMemoryBarrier.subresourceRange.setAspectMask(aspectMask);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::subresourceRange(vk::ImageSubresourceRange subresourceRange) {
        mImageMemoryBarrier.setSubresourceRange(subresourceRange);
        return *this;
    }

    // ImageMemoryBarrier
    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::oldLayout(vk::ImageLayout oldLayout) {
        mImageMemoryBarrier.setOldLayout(oldLayout);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::newLayout(vk::ImageLayout newLayout) {
        mImageMemoryBarrier.setNewLayout(newLayout);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::srcQueueFamilyIndex(uint32_t srcQueueFamilyIndex) {
        mImageMemoryBarrier.setSrcQueueFamilyIndex(srcQueueFamilyIndex);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::dstQueueFamilyIndex(uint32_t dstQueueFamilyIndex) {
        mImageMemoryBarrier.setDstQueueFamilyIndex(dstQueueFamilyIndex);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::srcAccessMask(vk::AccessFlags srcAccessMask) {
        mImageMemoryBarrier.setSrcAccessMask(srcAccessMask);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::dstAccessMask(vk::AccessFlags dstAccessMask) {
        mImageMemoryBarrier.setDstAccessMask(dstAccessMask);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::image(vk::Image image) {
        mImageMemoryBarrier.setImage(image);
        return *this;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::next(const void *next) {
        mImageMemoryBarrier.setPNext(next);
        return *this;
    }

    [[nodiscard]]
    vk::ImageMemoryBarrier ImageMemoryBarrierBuilder::build() const {
        if (mImageMemoryBarrier.image == nullptr) {
            throw std::runtime_error("ImageMemoryBarrierBuilder::build(): mImageMemoryBarrier.image == nullptr");
        }
        return mImageMemoryBarrier;
    }

    ImageMemoryBarrierBuilder &ImageMemoryBarrierBuilder::asDefault() {
        mImageMemoryBarrier.subresourceRange
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setAspectMask(vk::ImageAspectFlagBits::eColor);

        mImageMemoryBarrier
                .setOldLayout(vk::ImageLayout::eUndefined)
                .setNewLayout(vk::ImageLayout::eReadOnlyOptimal)
                .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setSrcAccessMask(vk::AccessFlagBits::eNone)
                .setDstAccessMask(vk::AccessFlagBits::eShaderRead);

        return *this;
    }

} // vklite
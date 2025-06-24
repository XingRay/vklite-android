//
// Created by leixing on 2025-06-22.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class ImageMemoryBarrierBuilder {
    private:
        vk::ImageMemoryBarrier mImageMemoryBarrier;

    public:
        ImageMemoryBarrierBuilder();

        ~ImageMemoryBarrierBuilder();

        // SubresourceRange
        ImageMemoryBarrierBuilder &baseMipmapLevel(uint32_t baseMipmapLevel);

        ImageMemoryBarrierBuilder &levelCount(uint32_t levelCount);

        ImageMemoryBarrierBuilder &baseArrayLayer(uint32_t baseArrayLayer);

        ImageMemoryBarrierBuilder &layerCount(uint32_t layerCount);

        ImageMemoryBarrierBuilder &aspectMask(vk::ImageAspectFlags aspectMask);

        ImageMemoryBarrierBuilder &subresourceRange(vk::ImageSubresourceRange subresourceRange);


        // ImageMemoryBarrier
        ImageMemoryBarrierBuilder &oldLayout(vk::ImageLayout oldLayout);

        ImageMemoryBarrierBuilder &newLayout(vk::ImageLayout newLayout);

        ImageMemoryBarrierBuilder &srcQueueFamilyIndex(uint32_t srcQueueFamilyIndex);

        ImageMemoryBarrierBuilder &dstQueueFamilyIndex(uint32_t dstQueueFamilyIndex);

        ImageMemoryBarrierBuilder &srcAccessMask(vk::AccessFlags srcAccessMask);

        ImageMemoryBarrierBuilder &dstAccessMask(vk::AccessFlags dstAccessMask);

        ImageMemoryBarrierBuilder &image(vk::Image image);

        ImageMemoryBarrierBuilder &next(const void *next);

        [[nodiscard]]
        vk::ImageMemoryBarrier build() const;

        ImageMemoryBarrierBuilder &asDefault();
    };

} // vklite

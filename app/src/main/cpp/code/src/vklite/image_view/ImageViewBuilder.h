//
// Created by leixing on 2025/5/7.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"
#include "vklite/image_view/ImageView.h"

namespace vklite {

    class ImageViewBuilder {
    private:
        vk::Device mDevice;
        vk::ImageViewCreateInfo mImageViewCreateInfo;

    public:
        ImageViewBuilder();

        ~ImageViewBuilder();

        ImageViewBuilder &device(vk::Device device);

        ImageViewBuilder &image(vk::Image image);

        ImageViewBuilder &aspectMask(vk::ImageAspectFlags aspectMask);

        ImageViewBuilder &baseMipLevel(uint32_t baseMipLevel);

        ImageViewBuilder &levelCount(uint32_t levelCount);

        ImageViewBuilder &baseArrayLayer(uint32_t baseArrayLayer);

        ImageViewBuilder &layerCount(uint32_t layerCount);

        ImageViewBuilder &componentMappingRed(vk::ComponentSwizzle mapping);

        ImageViewBuilder &componentMappingGreen(vk::ComponentSwizzle mapping);

        ImageViewBuilder &componentMappingBlue(vk::ComponentSwizzle mapping);

        ImageViewBuilder &componentMappingAlpha(vk::ComponentSwizzle mapping);

        ImageViewBuilder &viewType(vk::ImageViewType viewType);

        ImageViewBuilder &format(vk::Format format);

        ImageViewBuilder &next(const void *next);

        ImageView build();

        std::unique_ptr<ImageView> buildUnique();

        std::vector<ImageView> build(const std::vector<vk::Image> &images);

        ImageViewBuilder &asDefault();

        ImageViewBuilder &asColorAttachment();

        ImageViewBuilder &asDepthAttachment();

        ImageViewBuilder &asTexture();

        ImageViewBuilder &asStorageImage();
    };

} // vklite

//
// Created by leixing on 2025/5/7.
//

#pragma once

#include <memory>

#include "vulkan/vulkan.hpp"

#include "vklite/device/Device.h"
#include "vklite/image_view/ImageView.h"
#include "vklite/image/ImageInterface.h"

namespace vklite {

    class ImageViewBuilder {
    private:
        vk::ImageViewCreateInfo mImageViewCreateInfo;

    public:
        ImageViewBuilder();

        ~ImageViewBuilder();

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

        ImageView build(const Device &device, const vk::Image &image);

        std::unique_ptr<ImageView> buildUnique(const Device &device, const vk::Image &image);

        ImageView build(const Device &device, const ImageInterface &image);

        std::unique_ptr<ImageView> buildUnique(const Device &device, const ImageInterface &image);

        std::vector<ImageView> build(const Device &device, const std::vector<vk::Image> &images);

        std::vector<ImageView> build(const Device &device, const std::vector<ImageInterface> &images);

    public://static

        static ImageViewBuilder defaultImageViewBuilder();

        static ImageViewBuilder colorImageViewBuilder();

        static ImageViewBuilder depthImageViewBuilder();

    };

} // vklite

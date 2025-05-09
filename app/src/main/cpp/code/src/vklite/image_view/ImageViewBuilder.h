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

        vk::Format mFormat;
        vk::ImageAspectFlags mImageAspect;
        uint32_t mLevelCount;

    public:
        ImageViewBuilder();

        ~ImageViewBuilder();

        ImageViewBuilder &format(vk::Format format);

        ImageViewBuilder &imageAspect(vk::ImageAspectFlags imageAspect);

        ImageViewBuilder &levelCount(uint32_t levelCount);

        std::unique_ptr<ImageView> build(const Device &device, const vk::Image &image);

        std::unique_ptr<ImageView> build(const Device &device, const ImageInterface &image);

        std::vector<ImageView> build(const Device &device, const std::vector<vk::Image> &images);

    public://static
        static ImageViewBuilder colorImageViewBuilder();

        static ImageViewBuilder depthImageViewBuilder();

    };

} // vklite

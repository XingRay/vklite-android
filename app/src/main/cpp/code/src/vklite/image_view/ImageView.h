//
// Created by leixing on 2025/5/7.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "vklite/device/Device.h"

namespace vklite {

    class ImageView {
    private:
        const Device &mDevice;

        vk::ImageView mImageView;

    public:
        ImageView(const Device &device, const vk::Image &image, vk::Format format, vk::ImageAspectFlags imageAspectFlags, uint32_t levelCount);

        ~ImageView();

        [[nodiscard]]
        const vk::ImageView &getImageView() const;
    };

} // vklite

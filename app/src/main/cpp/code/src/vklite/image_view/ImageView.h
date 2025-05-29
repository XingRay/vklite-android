//
// Created by leixing on 2025/5/7.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/image_view/ImageViewInterface.h"

namespace vklite {

    class ImageView : public ImageViewInterface {
    private:
        vk::Device mDevice;
        vk::ImageView mImageView;

    public:
        ImageView(vk::Device device, vk::ImageView imageView);

        ~ImageView() override;

        ImageView(const ImageView &other) = delete;

        ImageView &operator=(const ImageView &other) = delete;

        ImageView(ImageView &&other) noexcept;

        ImageView &operator=(ImageView &&other) noexcept;

        [[nodiscard]]
        const vk::ImageView &getImageView() const override;
    };

} // vklite

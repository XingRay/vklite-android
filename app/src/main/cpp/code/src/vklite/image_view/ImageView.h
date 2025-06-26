//
// Created by leixing on 2025/5/7.
//

#pragma once

#include <vulkan/vulkan.hpp>


namespace vklite {

    class ImageView {
    private:
        vk::Device mDevice;
        vk::ImageView mImageView;

    public:
        explicit ImageView(vk::Device device, vk::ImageView imageView);

        ~ImageView();

        ImageView(const ImageView &other) = delete;

        ImageView &operator=(const ImageView &other) = delete;

        ImageView(ImageView &&other) noexcept;

        ImageView &operator=(ImageView &&other) noexcept;

        [[nodiscard]]
        const vk::ImageView &getVkImageView() const;
    };

} // vklite

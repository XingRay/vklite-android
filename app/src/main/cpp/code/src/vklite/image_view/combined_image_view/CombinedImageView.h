//
// Created by leixing on 2025/5/30.
//

#pragma once

#include <vulkan/vulkan.h>

#include "vklite/image/Image.h"
#include "vklite/device_memory/DeviceMemory.h"
#include "vklite/image_view/ImageView.h"

namespace vklite {

    class CombinedImageView {
    private:
        Image mImage;
        DeviceMemory mDeviceMemory;
        ImageView mImageView;

    public:
        explicit CombinedImageView(Image &&image, DeviceMemory &&deviceMemory, ImageView &&imageView);

        ~CombinedImageView();

        CombinedImageView(const CombinedImageView &other) = delete;

        CombinedImageView &operator=(const CombinedImageView &other) = delete;

        CombinedImageView(CombinedImageView &&other) noexcept;

        CombinedImageView &operator=(CombinedImageView &&other) noexcept;

        [[nodiscard]]
        Image &getImage();

        [[nodiscard]]
        const vk::Image &getVkImage() const;

        [[nodiscard]]
        DeviceMemory &getDeviceMemory();

        [[nodiscard]]
        const vk::DeviceMemory &getVkDeviceMemory();

        [[nodiscard]]
        ImageView &getImageView();

        [[nodiscard]]
        const vk::ImageView &getVkImageView();
    };

} // vklite

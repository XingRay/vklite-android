//
// Created by leixing on 2025/5/17.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class ImageViewInterface {
    private:

    public:
        ImageViewInterface();

        virtual ~ImageViewInterface() = 0;

        [[nodiscard]]
        virtual const vk::ImageView &getImageView() const = 0;
    };

} // vklite

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

        // 声明移动构造函数和移动赋值运算符
        ImageViewInterface(ImageViewInterface &&) noexcept = default;

        ImageViewInterface &operator=(ImageViewInterface &&) noexcept = default;

        [[nodiscard]]
        virtual const vk::ImageView &getImageView() const = 0;
    };

} // vklite

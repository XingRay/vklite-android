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

        ImageView() = delete;

        ImageView(const Device &device, const vk::Image &image, vk::Format format, vk::ImageAspectFlags imageAspectFlags, uint32_t levelCount);

        // 拷贝构造函数（显式删除） ---
        // 原因：mDevice 是 const 引用，无法重新绑定；
        ImageView(const ImageView &) = delete;

        // 移动构造函数
        ImageView(ImageView &&other) noexcept;

        // 拷贝赋值运算符
        ImageView &operator=(const ImageView &) = delete;

        // 移动赋值运算符
        ImageView &operator=(ImageView &&) = delete;

        ~ImageView();

        [[nodiscard]]
        const vk::ImageView &getImageView() const;
    };

} // vklite

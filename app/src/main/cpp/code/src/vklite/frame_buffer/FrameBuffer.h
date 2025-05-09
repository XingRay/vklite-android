//
// Created by leixing on 2024/12/31.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/swapchain/Swapchain.h"
#include "vklite/render_pass/RenderPass.h"
#include "vklite/command_pool/CommandPool.h"
#include "vklite/image_view/ImageView.h"

namespace vklite {

    class FrameBuffer {
    private:
        const Device &mDevice;
        vk::Framebuffer mFrameBuffer;

    public:

        // 默认构造函数（不可行）, 因为 const 引用成员 mDevice 必须初始化，且无默认构造函数
        FrameBuffer() = delete;

        // 带参数的构造函数
        FrameBuffer(const Device &device, const RenderPass &renderPass,
                    const std::vector<vk::ImageView> &attachments, uint32_t width, uint32_t height, uint32_t layers);

        // 拷贝构造函数（显式删除） ---
        // 原因：mDevice 是 const 引用，无法重新绑定；
        FrameBuffer(const FrameBuffer &) = delete;

        // 移动构造函数
        FrameBuffer(FrameBuffer &&other) noexcept;

        // 拷贝赋值运算符
        FrameBuffer &operator=(const FrameBuffer &) = delete;

        // 移动赋值运算符
        FrameBuffer &operator=(FrameBuffer &&) = delete;

        ~FrameBuffer();

        [[nodiscard]]
        const vk::Framebuffer &getFrameBuffer() const;

    private:

    };

} // vklite

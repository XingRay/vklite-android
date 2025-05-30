//
// Created by leixing on 2024/12/31.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include "vklite/swapchain/Swapchain.h"
#include "vklite/render_pass/RenderPass.h"
#include "vklite/command_pool/CommandPool.h"
#include "vklite/image_view/ImageView.h"

namespace vklite {

    class FrameBuffer {
    private:
        vk::Device mDevice;
        vk::Framebuffer mFrameBuffer;

    public:
        FrameBuffer(vk::Device device, vk::Framebuffer frameBuffer);

        ~FrameBuffer();

        FrameBuffer(const FrameBuffer &other) = delete;

        FrameBuffer &operator=(const FrameBuffer &&other) = delete;

        FrameBuffer(FrameBuffer &&other) noexcept;

        FrameBuffer &operator=(FrameBuffer &&other) noexcept;

        [[nodiscard]]
        const vk::Framebuffer &getFrameBuffer() const;
    };

} // vklite

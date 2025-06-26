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

    class Framebuffer {
    private:
        vk::Device mDevice;
        vk::Framebuffer mFramebuffer;

    public:
        explicit Framebuffer(vk::Device device, vk::Framebuffer frameBuffer);

        ~Framebuffer();

        Framebuffer(const Framebuffer &other) = delete;

        Framebuffer &operator=(const Framebuffer &&other) = delete;

        Framebuffer(Framebuffer &&other) noexcept;

        Framebuffer &operator=(Framebuffer &&other) noexcept;

        [[nodiscard]]
        const vk::Framebuffer &getVkFramebuffer() const;
        
    };

} // vklite

//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>

#include "vklite/frame_buffer/Framebuffer.h"
#include "vklite/image_view/ImageView.h"

namespace vklite {

    class FramebufferBuilder {
    private:
        vk::Device mDevice;
        vk::FramebufferCreateInfo mFramebufferCreateInfo;

        std::vector<vk::ImageView> mAttachments;

    public:
        FramebufferBuilder();

        ~FramebufferBuilder();

        FramebufferBuilder &device(vk::Device device);

        FramebufferBuilder &renderPass(vk::RenderPass renderPass);

        FramebufferBuilder &width(uint32_t width);

        FramebufferBuilder &height(uint32_t height);

        FramebufferBuilder &size(vk::Extent2D size);

        FramebufferBuilder &layers(uint32_t layers);

        FramebufferBuilder &addAttachment(vk::ImageView attachment);

        FramebufferBuilder &addAttachmentIf(bool condition, vk::ImageView attachment);

        FramebufferBuilder &addAttachmentIf(bool condition, const std::function<vk::ImageView()> &attachment);

        [[nodiscard]]
        Framebuffer build();

        [[nodiscard]]
        std::unique_ptr<Framebuffer> buildUnique();
    };

} // vklite

//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>

#include "vklite/frame_buffer/FrameBuffer.h"
#include "vklite/image_view/ImageView.h"

namespace vklite {

    class FrameBufferBuilder {
    private:
        vk::Device mDevice;
        vk::FramebufferCreateInfo mFramebufferCreateInfo;

        std::vector<vk::ImageView> mAttachments;

    public:
        FrameBufferBuilder();

        ~FrameBufferBuilder();

        FrameBufferBuilder &device(vk::Device device);

        FrameBufferBuilder &renderPass(vk::RenderPass renderPass);

        FrameBufferBuilder &width(uint32_t width);

        FrameBufferBuilder &height(uint32_t height);

        FrameBufferBuilder &layers(uint32_t layers);

        FrameBufferBuilder &addAttachment(vk::ImageView attachment);

        FrameBufferBuilder &addAttachmentIf(bool condition, vk::ImageView attachment);

        FrameBufferBuilder &addAttachmentIf(bool condition, const std::function<vk::ImageView()> &attachment);

        [[nodiscard]]
        FrameBuffer build();

        [[nodiscard]]
        std::unique_ptr<FrameBuffer> buildUnique();
    };

} // vklite

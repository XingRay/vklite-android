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

        uint32_t mWidth;
        uint32_t mHeight;
        uint32_t mLayers;
        std::vector<vk::ImageView> mAttachments;

    public:
        FrameBufferBuilder();

        ~FrameBufferBuilder();

        FrameBufferBuilder &width(uint32_t width);

        FrameBufferBuilder &height(uint32_t height);

        FrameBufferBuilder &layers(uint32_t layers);

        FrameBufferBuilder &addAttachment(vk::ImageView attachment);

        FrameBufferBuilder &addAttachmentIf(bool condition, vk::ImageView attachment);

        [[nodiscard]]
        FrameBuffer build(const Device &device, const RenderPass &renderPass);

        [[nodiscard]]
        std::unique_ptr<FrameBuffer> buildUnique(const Device &device, const RenderPass &renderPass);
    };

} // vklite

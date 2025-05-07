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
        vk::Format mDisplayFormat;
        vk::Extent2D mDisplaySize;
        std::vector<ImageView> mDisplayImageViews;
        vk::SampleCountFlagBits mSampleCountFlagBits;

    public:
        FrameBufferBuilder();

        ~FrameBufferBuilder();

        FrameBufferBuilder &displayFormat(vk::Format displayFormat);

        FrameBufferBuilder &displaySize(vk::Extent2D displaySize);

        FrameBufferBuilder &displayImageViews(std::vector<vklite::ImageView> &&mDisplayImageViews);

        FrameBufferBuilder &sampleCountFlagBits(vk::SampleCountFlagBits sampleCountFlagBits);

        [[nodiscard]]
        std::unique_ptr<FrameBuffer> build(const Device &device, const RenderPass &renderPass, const CommandPool &commandPool);
    };

} // vklite

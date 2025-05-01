//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>

#include "vklite/frame_buffer/FrameBuffer.h"

namespace vklite {

    class FrameBufferBuilder {
    private:
        vk::Format mDisplayFormat;
        vk::Extent2D mDisplaySize;
        vk::SampleCountFlagBits mSampleCountFlagBits;

    public:
        FrameBufferBuilder();

        ~FrameBufferBuilder();

        FrameBufferBuilder &displayFormat(vk::Format displayFormat);

        FrameBufferBuilder &displaySize(vk::Extent2D displaySize);

        FrameBufferBuilder &sampleCountFlagBits(vk::SampleCountFlagBits sampleCountFlagBits);

        [[nodiscard]]
        std::unique_ptr<FrameBuffer> build(const Device &device, const RenderPass &renderPass, const CommandPool &commandPool,
                                           const std::vector<vk::ImageView> &displayImageViews);
    };

} // vklite

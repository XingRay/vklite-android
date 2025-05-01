//
// Created by leixing on 2025/5/1.
//

#include "FrameBufferBuilder.h"

namespace vklite {

    FrameBufferBuilder::FrameBufferBuilder() {

    }

    FrameBufferBuilder::~FrameBufferBuilder() {

    }

    FrameBufferBuilder &FrameBufferBuilder::displayFormat(vk::Format displayFormat) {
        mDisplayFormat = displayFormat;
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::displaySize(vk::Extent2D displaySize) {
        mDisplaySize = displaySize;
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::sampleCountFlagBits(vk::SampleCountFlagBits sampleCountFlagBits) {
        mSampleCountFlagBits = sampleCountFlagBits;
        return *this;
    }

    std::unique_ptr<FrameBuffer> FrameBufferBuilder::build(const Device &device, const RenderPass &renderPass, const CommandPool &commandPool,
                                                           const std::vector<vk::ImageView> &displayImageViews) {

        return std::make_unique<FrameBuffer>(device, renderPass, commandPool, displayImageViews, mDisplayFormat, mDisplaySize, mSampleCountFlagBits);
    }
} // vklite
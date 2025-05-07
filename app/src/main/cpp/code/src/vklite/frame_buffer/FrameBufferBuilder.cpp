//
// Created by leixing on 2025/5/1.
//

#include "FrameBufferBuilder.h"

namespace vklite {

    FrameBufferBuilder::FrameBufferBuilder()
            : mSampleCountFlagBits(vk::SampleCountFlagBits::e1) {}

    FrameBufferBuilder::~FrameBufferBuilder() = default;

    FrameBufferBuilder &FrameBufferBuilder::displayFormat(vk::Format displayFormat) {
        mDisplayFormat = displayFormat;
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::displaySize(vk::Extent2D displaySize) {
        mDisplaySize = displaySize;
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::displayImageViews(std::vector<vklite::ImageView> &&displayImageViews) {
        mDisplayImageViews = std::move(displayImageViews);
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::sampleCountFlagBits(vk::SampleCountFlagBits sampleCountFlagBits) {
        mSampleCountFlagBits = sampleCountFlagBits;
        return *this;
    }

    std::unique_ptr<FrameBuffer> FrameBufferBuilder::build(const Device &device, const RenderPass &renderPass, const CommandPool &commandPool) {
        return std::make_unique<FrameBuffer>(device, renderPass, commandPool, std::move(mDisplayImageViews), mDisplayFormat, mDisplaySize, mSampleCountFlagBits);
    }
} // vklite
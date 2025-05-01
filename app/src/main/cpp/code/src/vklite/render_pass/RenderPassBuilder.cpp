//
// Created by leixing on 2025/5/1.
//

#include "RenderPassBuilder.h"

namespace vklite {

    RenderPassBuilder::RenderPassBuilder()
            : mSampleCountFlagBits(vk::SampleCountFlagBits::e1) {}

    RenderPassBuilder::~RenderPassBuilder() = default;

    RenderPassBuilder &RenderPassBuilder::sampleCount(vk::SampleCountFlagBits sampleCountFlagBits) {
        mSampleCountFlagBits = sampleCountFlagBits;
        return *this;
    }

    std::unique_ptr<RenderPass> RenderPassBuilder::build(const Device &device, const Swapchain &swapchain) {
        return std::make_unique<RenderPass>(device, swapchain, mSampleCountFlagBits);
    }

} // vklite
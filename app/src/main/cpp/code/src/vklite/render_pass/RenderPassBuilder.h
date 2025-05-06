//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>

#include "vklite/render_pass/RenderPass.h"

namespace vklite {

    class RenderPassBuilder {
    private:
        bool mEnableMsaa;
        vk::SampleCountFlagBits mSampleCountFlagBits;

        bool mEnableDepth;

        vk::Format mDisplayFormat;
    public:
        RenderPassBuilder();

        ~RenderPassBuilder();

        RenderPassBuilder &enableMsaa();

        RenderPassBuilder &enableDepth();

        RenderPassBuilder &sampleCountFlagBits(vk::SampleCountFlagBits mSampleCountFlagBits);

        RenderPassBuilder &displayFormat(vk::Format displayFormat);

        std::unique_ptr<RenderPass> build(const Device &device, const Swapchain &swapchain);
    };

} // vklite

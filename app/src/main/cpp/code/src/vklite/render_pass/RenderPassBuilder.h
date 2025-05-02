//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>

#include "vklite/render_pass/RenderPass.h"

namespace vklite {

    class RenderPassBuilder {
    private:
        vk::SampleCountFlagBits mSampleCountFlagBits;

    public:
        RenderPassBuilder();

        ~RenderPassBuilder();

        RenderPassBuilder &sampleCountFlagBits(vk::SampleCountFlagBits mSampleCountFlagBits);

        std::unique_ptr<RenderPass> build(const Device &device, const Swapchain &swapchain);
    };

} // vklite

//
// Created by leixing on 2025/5/19.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"
#include "vklite/image/ImageBuilder.h"
#include "vklite/image/depth_image/DepthImage.h"
#include "vklite/swapchain/Swapchain.h"

namespace vklite {

    class DepthImageBuilder {

    private:
        ImageBuilder mImageBuilder;

    public:
        DepthImageBuilder();

        ~DepthImageBuilder();

        DepthImageBuilder &width(uint32_t width);

        DepthImageBuilder &height(uint32_t height);

        DepthImageBuilder &format(vk::Format format);

        DepthImageBuilder &setSizeAndFormat(const Swapchain &swapchain);

        DepthImageBuilder &sampleCount(vk::SampleCountFlagBits sampleCount);

        [[nodiscard]]
        DepthImage build(const Device &device);

        [[nodiscard]]
        std::unique_ptr<DepthImage> buildUnique(const Device &device);
    };

} // vklite

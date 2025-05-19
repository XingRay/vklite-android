//
// Created by leixing on 2025/5/19.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"
#include "vklite/image/ImageBuilder.h"
#include "vklite/image/color_image/ColorImage.h"
#include "vklite/swapchain/Swapchain.h"

namespace vklite {

    class ColorImageBuilder {

    private:
        ImageBuilder mImageBuilder;

    public:
        ColorImageBuilder();

        ~ColorImageBuilder();

        ColorImageBuilder &width(uint32_t width);

        ColorImageBuilder &height(uint32_t height);

        ColorImageBuilder &format(vk::Format format);

        ColorImageBuilder &setSizeAndFormat(const Swapchain& swapchain);

        ColorImageBuilder &sampleCount(vk::SampleCountFlagBits sampleCount);

        [[nodiscard]]
        ColorImage build(const Device &device);

        [[nodiscard]]
        std::unique_ptr<ColorImage> buildUnique(const Device &device);
    };

} // vklite

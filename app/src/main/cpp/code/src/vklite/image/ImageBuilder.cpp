//
// Created by leixing on 2025/4/11.
//

#include "ImageBuilder.h"
#include "Image.h"

namespace vklite {

    ImageBuilder::ImageBuilder() = default;

    ImageBuilder::~ImageBuilder() = default;

    std::unique_ptr<ImageInterface> ImageBuilder::build(const Device &device, uint32_t width, uint32_t height, vk::Format format) {
        uint32_t mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        return std::make_unique<Image>(device, width, height, format, mipLevels);
    }
} // vklite
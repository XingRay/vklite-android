//
// Created by leixing on 2025/4/11.
//

#include "ImageBuilder.h"
#include "Image.h"

namespace vklite {
    ImageBuilder::ImageBuilder() {

    }

    ImageBuilder::~ImageBuilder() {

    }

    std::unique_ptr<ImageInterface> ImageBuilder::build(const Device &device, uint32_t width, uint32_t height, vk::Format format) {
        return std::make_unique<Image>(device, width, height, format);
    }
} // vklite
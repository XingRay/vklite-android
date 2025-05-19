//
// Created by leixing on 2025/5/19.
//

#include "DepthImageBuilder.h"

namespace vklite {

    DepthImageBuilder::DepthImageBuilder()
            : mImageBuilder(ImageBuilder::colorImageBuilder()) {}

    DepthImageBuilder::~DepthImageBuilder() = default;

    DepthImageBuilder &DepthImageBuilder::width(uint32_t width) {
        mImageBuilder.width(width);
        return *this;
    }

    DepthImageBuilder &DepthImageBuilder::height(uint32_t height) {
        mImageBuilder.height(height);
        return *this;
    }

    DepthImageBuilder &DepthImageBuilder::format(vk::Format format) {
        mImageBuilder.format(format);
        return *this;
    }

    DepthImageBuilder &DepthImageBuilder::setSizeAndFormat(const Swapchain &swapchain) {
        vk::Extent2D extent = swapchain.getDisplaySize();
        (*this)
                .width(extent.width)
                .height(extent.height)
                .format(swapchain.getDisplayFormat());
        return *this;
    }

    DepthImageBuilder &DepthImageBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mImageBuilder.sampleCount(sampleCount);
        return *this;
    }

    DepthImage DepthImageBuilder::build(const Device &device) {
        return DepthImage(mImageBuilder.build(device));
    }

    std::unique_ptr<DepthImage> DepthImageBuilder::buildUnique(const Device &device) {
        return std::make_unique<DepthImage>(mImageBuilder.build(device));
    }

} // vklite
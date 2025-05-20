//
// Created by leixing on 2025/5/19.
//

#include "ColorImageBuilder.h"

namespace vklite {

    ColorImageBuilder::ColorImageBuilder(ImageBuilder imageBuilder)
            : mImageBuilder(imageBuilder) {}

    ColorImageBuilder::ColorImageBuilder()
            : ColorImageBuilder(ImageBuilder::colorImageBuilder()) {}

    ColorImageBuilder::~ColorImageBuilder() = default;

    ColorImageBuilder &ColorImageBuilder::width(uint32_t width) {
        mImageBuilder.width(width);
        return *this;
    }

    ColorImageBuilder &ColorImageBuilder::height(uint32_t height) {
        mImageBuilder.height(height);
        return *this;
    }

    ColorImageBuilder &ColorImageBuilder::format(vk::Format format) {
        mImageBuilder.format(format);
        return *this;
    }

    ColorImageBuilder &ColorImageBuilder::size(const vk::Extent2D &extent) {
        (*this)
                .width(extent.width)
                .height(extent.height);
        return *this;
    }

    ColorImageBuilder &ColorImageBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mImageBuilder.sampleCount(sampleCount);
        return *this;
    }

    ColorImage ColorImageBuilder::build(const Device &device) {
        return ColorImage(mImageBuilder.build(device));
    }

    std::unique_ptr<ColorImage> ColorImageBuilder::buildUnique(const Device &device) {
        return std::make_unique<ColorImage>(mImageBuilder.build(device));
    }

} // vklite
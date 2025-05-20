//
// Created by leixing on 2025/5/19.
//

#include "DepthImageBuilder.h"

namespace vklite {

    DepthImageBuilder::DepthImageBuilder(const ImageBuilder &imageBuilder)
            : mImageBuilder(imageBuilder) {}

    DepthImageBuilder::DepthImageBuilder()
            : DepthImageBuilder(ImageBuilder::depthImageBuilder()) {}

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

    DepthImageBuilder &DepthImageBuilder::size(const vk::Extent2D &size) {
        (*this)
                .width(size.width)
                .height(size.height);
        return *this;
    }

    DepthImageBuilder &DepthImageBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mImageBuilder.sampleCount(sampleCount);
        return *this;
    }

    DepthImageBuilder &DepthImageBuilder::postCreated(std::function<void(DepthImage &)> &&postCreatedHandler) {
        mPostCreatedHandler = std::move(postCreatedHandler);
        return *this;
    }

    DepthImage DepthImageBuilder::build(const Device &device) {
        DepthImage depthImage = DepthImage(mImageBuilder.build(device));
        if (mPostCreatedHandler != nullptr) {
            mPostCreatedHandler(depthImage);
        }
        return std::move(depthImage);
    }

    std::unique_ptr<DepthImage> DepthImageBuilder::buildUnique(const Device &device) {
        std::unique_ptr<DepthImage> depthImage = std::make_unique<DepthImage>(mImageBuilder.build(device));
        if (mPostCreatedHandler != nullptr) {
            mPostCreatedHandler(*depthImage);
        }
        return depthImage;
    }

} // vklite
//
// Created by leixing on 2025/4/9.
//

#include "ImageInfo.h"

namespace vklite {

    ImageInfo::ImageInfo(std::unique_ptr<ImageInterface> &&image,
                                                         std::unique_ptr<SamplerInterface> &&sampler)
            : mImage(std::move(image)),
              mSampler(std::move(sampler)) {}

    ImageInfo::~ImageInfo() = default;

    const std::unique_ptr<ImageInterface> &ImageInfo::getImage() const {
        return mImage;
    }

    const std::unique_ptr<SamplerInterface> &ImageInfo::getSampler() const {
        return mSampler;
    }

    [[nodiscard]]
    vk::DescriptorImageInfo ImageInfo::createDescriptorImageInfo() const {
        vk::DescriptorImageInfo descriptorImageInfo{};
        descriptorImageInfo
                .setSampler(mSampler->getSampler())
//                .setImageView(mImage->getImageView())
                        // todo: set by field
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);

        return descriptorImageInfo;
    }

} // vklite
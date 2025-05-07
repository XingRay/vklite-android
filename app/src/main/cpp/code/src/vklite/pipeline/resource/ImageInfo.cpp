//
// Created by leixing on 2025/4/9.
//

#include "ImageInfo.h"

namespace vklite {

    ImageInfo::ImageInfo(std::unique_ptr<ImageInterface> &&vulkanImage,
                                                         std::unique_ptr<SamplerInterface> &&vulkanSampler)
            : mImage(std::move(vulkanImage)),
              mSampler(std::move(vulkanSampler)) {}

    ImageInfo::~ImageInfo() = default;

    const std::unique_ptr<ImageInterface> &ImageInfo::getVulkanImage() const {
        return mImage;
    }

    const std::unique_ptr<SamplerInterface> &ImageInfo::getVulkanSampler() const {
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
//
// Created by leixing on 2025/4/9.
//

#pragma once

#include <memory>
#include "vklite/image/ImageInterface.h"
#include "vklite/sampler/SamplerInterface.h"

namespace vklite {

    class ImageInfo {
    private:
        std::unique_ptr<ImageInterface> mImage;
        std::unique_ptr<SamplerInterface> mSampler;

    public:
        ImageInfo(std::unique_ptr<ImageInterface> &&vulkanImage,
                  std::unique_ptr<SamplerInterface> &&vulkanSampler);

        ~ImageInfo();

        const std::unique_ptr<ImageInterface> &getVulkanImage() const;

        const std::unique_ptr<SamplerInterface> &getVulkanSampler() const;

        [[nodiscard]]
        vk::DescriptorImageInfo createDescriptorImageInfo() const;
    };

} // vklite

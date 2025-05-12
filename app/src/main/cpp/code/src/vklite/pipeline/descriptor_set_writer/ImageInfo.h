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
        ImageInfo(std::unique_ptr<ImageInterface> &&image,
                  std::unique_ptr<SamplerInterface> &&sampler);

        ~ImageInfo();

        const std::unique_ptr<ImageInterface> &getImage() const;

        const std::unique_ptr<SamplerInterface> &getSampler() const;

        [[nodiscard]]
        vk::DescriptorImageInfo createDescriptorImageInfo() const;
    };

} // vklite

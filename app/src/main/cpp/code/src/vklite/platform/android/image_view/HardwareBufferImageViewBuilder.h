//
// Created by leixing on 2025/5/21.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/image_view/ImageViewBuilder.h"
#include "vklite/device/Device.h"
#include "vklite/image_view/ImageView.h"
#include "vklite/image/Image.h"

namespace vklite {

    class HardwareBufferImageViewBuilder {
    private:
        ImageViewBuilder mImageViewBuilder;
        vk::SamplerYcbcrConversionInfo mSamplerYcbcrConversionInfo;

    public:
        HardwareBufferImageViewBuilder(ImageViewBuilder imageViewBuilder,
                                       vk::SamplerYcbcrConversionInfo samplerYcbcrConversionInfo);

        HardwareBufferImageViewBuilder();

        ~HardwareBufferImageViewBuilder();

        ImageViewBuilder &getImageViewBuilder();

        HardwareBufferImageViewBuilder &conversion(const vk::SamplerYcbcrConversion &conversion);

        HardwareBufferImageViewBuilder &format(vk::Format format);

        ImageView build(const Device &device, const vk::Image &image);

        std::unique_ptr<ImageView> buildUnique(const Device &device, const vk::Image &image);

        ImageView build(const Device &device, const Image &image);

        std::unique_ptr<ImageView> buildUnique(const Device &device, const Image &image);
    };

} // vklite

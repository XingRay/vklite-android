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
        vk::Device mDevice;
        ImageViewBuilder mImageViewBuilder;
        vk::SamplerYcbcrConversionInfo mSamplerYcbcrConversionInfo;

    public:
        explicit HardwareBufferImageViewBuilder();

        ~HardwareBufferImageViewBuilder();

        HardwareBufferImageViewBuilder &device(vk::Device device);

        HardwareBufferImageViewBuilder &image(vk::Image image);

        HardwareBufferImageViewBuilder &conversion(const vk::SamplerYcbcrConversion &conversion);

        HardwareBufferImageViewBuilder &format(vk::Format format);

        ImageView build();

        std::unique_ptr<ImageView> buildUnique();
    };

} // vklite

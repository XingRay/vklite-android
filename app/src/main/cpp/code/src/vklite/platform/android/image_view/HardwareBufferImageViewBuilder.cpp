//
// Created by leixing on 2025/5/21.
//

#include "HardwareBufferImageViewBuilder.h"

namespace vklite {

    HardwareBufferImageViewBuilder::HardwareBufferImageViewBuilder() {
        mImageViewBuilder.asDefault()
                .aspectMask(vk::ImageAspectFlagBits::eColor)
                .viewType(vk::ImageViewType::e2D);
        mImageViewBuilder.next(&mSamplerYcbcrConversionInfo);
    }

    HardwareBufferImageViewBuilder::~HardwareBufferImageViewBuilder() = default;

    HardwareBufferImageViewBuilder &HardwareBufferImageViewBuilder::device(vk::Device device) {
        mDevice = device;
        mImageViewBuilder.device(device);
        return *this;
    }

    HardwareBufferImageViewBuilder &HardwareBufferImageViewBuilder::image(vk::Image image) {
        mImageViewBuilder.image(image);
        return *this;
    }

    HardwareBufferImageViewBuilder &HardwareBufferImageViewBuilder::conversion(const vk::SamplerYcbcrConversion &conversion) {
        mSamplerYcbcrConversionInfo.setConversion(conversion);
        return *this;
    }

    HardwareBufferImageViewBuilder &HardwareBufferImageViewBuilder::format(vk::Format format) {
        mImageViewBuilder.format(format);
        return *this;
    }

    ImageView HardwareBufferImageViewBuilder::build() {
        return mImageViewBuilder.build();
    }

    std::unique_ptr<ImageView> HardwareBufferImageViewBuilder::buildUnique() {
        return std::make_unique<ImageView>(build());
    }

} // vklite
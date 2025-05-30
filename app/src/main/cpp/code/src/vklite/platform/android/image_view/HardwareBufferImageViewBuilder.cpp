//
// Created by leixing on 2025/5/21.
//

#include "HardwareBufferImageViewBuilder.h"

namespace vklite {

    HardwareBufferImageViewBuilder::HardwareBufferImageViewBuilder(ImageViewBuilder imageViewBuilder,
                                                                   vk::SamplerYcbcrConversionInfo samplerYcbcrConversionInfo)
            : mImageViewBuilder(imageViewBuilder), mSamplerYcbcrConversionInfo(samplerYcbcrConversionInfo) {}

    HardwareBufferImageViewBuilder::HardwareBufferImageViewBuilder()
            : mImageViewBuilder(ImageViewBuilder::defaultImageViewBuilder()
                                        .aspectMask(vk::ImageAspectFlagBits::eColor)
                                        .viewType(vk::ImageViewType::e2D)),
              mSamplerYcbcrConversionInfo{} {}

    HardwareBufferImageViewBuilder::~HardwareBufferImageViewBuilder() = default;

    ImageViewBuilder &HardwareBufferImageViewBuilder::getImageViewBuilder() {
        return mImageViewBuilder;
    }

    HardwareBufferImageViewBuilder &HardwareBufferImageViewBuilder::conversion(const vk::SamplerYcbcrConversion &conversion) {
        mSamplerYcbcrConversionInfo.setConversion(conversion);
        return *this;
    }

    HardwareBufferImageViewBuilder &HardwareBufferImageViewBuilder::format(vk::Format format) {
        mImageViewBuilder.format(format);
        return *this;
    }

    ImageView HardwareBufferImageViewBuilder::build(const Device &device, const vk::Image &image) {
        return mImageViewBuilder.next(&mSamplerYcbcrConversionInfo).device(device.getDevice()).image(image).build();
    }

    ImageView HardwareBufferImageViewBuilder::build(const Device &device, const Image &image) {
        return mImageViewBuilder.next(&mSamplerYcbcrConversionInfo).device(device.getDevice()).image(image.getImage()).build();
    }

    std::unique_ptr<ImageView> HardwareBufferImageViewBuilder::buildUnique(const Device &device, const vk::Image &image) {
        return mImageViewBuilder.next(&mSamplerYcbcrConversionInfo).device(device.getDevice()).image(image).buildUnique();
    }

} // vklite
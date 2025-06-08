//
// Created by leixing on 2025/6/7.
//

#include "CombinedHardwareBufferSampler.h"

namespace vklite {

    CombinedHardwareBufferSampler::CombinedHardwareBufferSampler(SamplerYcbcrConversion &&conversion,
                                                                 Image &&image,
                                                                 DeviceMemory &&deviceMemory,
                                                                 ImageView &&imageView,
                                                                 Sampler &&sampler)
            : mConversion(std::move(conversion)),
              mImage(std::move(image)),
              mDeviceMemory(std::move(deviceMemory)),
              mImageView(std::move(imageView)),
              mSampler(std::move(sampler)) {}

    CombinedHardwareBufferSampler::~CombinedHardwareBufferSampler() = default;

    CombinedHardwareBufferSampler::CombinedHardwareBufferSampler(CombinedHardwareBufferSampler &&other) noexcept
            : mConversion(std::move(other.mConversion)),
              mImage(std::move(other.mImage)),
              mDeviceMemory(std::move(other.mDeviceMemory)),
              mImageView(std::move(other.mImageView)),
              mSampler(std::move(other.mSampler)) {}

    CombinedHardwareBufferSampler &CombinedHardwareBufferSampler::operator=(CombinedHardwareBufferSampler &&other) noexcept {
        if (this != &other) {
            mConversion = std::move(other.mConversion);
            mImage = std::move(other.mImage);
            mDeviceMemory = std::move(other.mDeviceMemory);
            mImageView = std::move(other.mImageView);
            mSampler = std::move(other.mSampler);
        }
        return *this;
    }

    const SamplerYcbcrConversion &CombinedHardwareBufferSampler::getConversion() const {
        return mConversion;
    }

    const Image &CombinedHardwareBufferSampler::getImage() const {
        return mImage;
    }

    const DeviceMemory &CombinedHardwareBufferSampler::getDeviceMemory() const {
        return mDeviceMemory;
    }

    const ImageView &CombinedHardwareBufferSampler::getImageView() const {
        return mImageView;
    }

    const Sampler &CombinedHardwareBufferSampler::getSampler() const {
        return mSampler;
    }

    const vk::Sampler &CombinedHardwareBufferSampler::getVkSampler() const {
        return mSampler.getSampler();
    }

} // vklite
//
// Created by leixing on 2025/6/7.
//

#include "CombinedHardwareBufferSampler.h"

namespace vklite {

    CombinedHardwareBufferSampler::CombinedHardwareBufferSampler(SamplerYcbcrConversion &&conversion,
                                                                 Sampler &&sampler)
            : mConversion(std::move(conversion)),
              mSampler(std::move(sampler)) {}

    CombinedHardwareBufferSampler::~CombinedHardwareBufferSampler() = default;

    CombinedHardwareBufferSampler::CombinedHardwareBufferSampler(CombinedHardwareBufferSampler &&other) noexcept
            : mConversion(std::move(other.mConversion)),
              mSampler(std::move(other.mSampler)) {}

    CombinedHardwareBufferSampler &CombinedHardwareBufferSampler::operator=(CombinedHardwareBufferSampler &&other) noexcept {
        if (this != &other) {
            mConversion = std::move(other.mConversion);
            mSampler = std::move(other.mSampler);
        }
        return *this;
    }

    const SamplerYcbcrConversion &CombinedHardwareBufferSampler::getSamplerYcbcrConversion() const {
        return mConversion;
    }

    const vk::SamplerYcbcrConversion &CombinedHardwareBufferSampler::getVkSamplerYcbcrConversion() const {
        return mConversion.getVkSamplerYcbcrConversion();
    }

    const Sampler &CombinedHardwareBufferSampler::getSampler() const {
        return mSampler;
    }

    const vk::Sampler &CombinedHardwareBufferSampler::getVkSampler() const {
        return mSampler.getVkSampler();
    }

} // vklite
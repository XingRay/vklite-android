//
// Created by leixing on 2025/6/7.
//

#include "CombinedHardwareBufferSampler.h"

namespace vklite {

    CombinedHardwareBufferSampler::CombinedHardwareBufferSampler(Sampler &&sampler, HardwareBufferYcbcrConversion &&conversion)
            : mSampler(std::move(sampler)),
              mConversion(std::move(conversion)) {}

    CombinedHardwareBufferSampler::~CombinedHardwareBufferSampler() = default;

    CombinedHardwareBufferSampler::CombinedHardwareBufferSampler(CombinedHardwareBufferSampler &&other) noexcept
            : mSampler(std::move(other.mSampler)),
              mConversion(std::move(other.mConversion)) {}

    CombinedHardwareBufferSampler &CombinedHardwareBufferSampler::operator=(CombinedHardwareBufferSampler &&other) noexcept {
        if (this != &other) {
            mSampler = std::move(other.mSampler);
            mConversion = std::move(other.mConversion);
        }
        return *this;
    }

    const Sampler &CombinedHardwareBufferSampler::getSampler() const {
        return mSampler;
    }

    const vk::Sampler &CombinedHardwareBufferSampler::getVkSampler() const {
        return mSampler.getSampler();
    }

    const HardwareBufferYcbcrConversion &CombinedHardwareBufferSampler::getConversion() const {
        return mConversion;
    }

} // vklite
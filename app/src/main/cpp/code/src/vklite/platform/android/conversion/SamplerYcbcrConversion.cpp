//
// Created by leixing on 2025/3/15.
//

#include "SamplerYcbcrConversion.h"
#include "vklite/util/VkCheck.h"

namespace vklite {

    SamplerYcbcrConversion::SamplerYcbcrConversion(vk::Device device, vk::SamplerYcbcrConversion samplerYcbcrConversion)
            : mDevice(device), mSamplerYcbcrConversion(samplerYcbcrConversion) {}

    SamplerYcbcrConversion::~SamplerYcbcrConversion() {
        if (mDevice != nullptr && mSamplerYcbcrConversion != nullptr) {
            // device.destroySamplerYcbcrConversion(mConversion); // link error
            vkDestroySamplerYcbcrConversion(mDevice, mSamplerYcbcrConversion, nullptr);
            mDevice = nullptr;
            mSamplerYcbcrConversion = nullptr;
        }
    }

    SamplerYcbcrConversion::SamplerYcbcrConversion(SamplerYcbcrConversion &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mSamplerYcbcrConversion(std::exchange(other.mSamplerYcbcrConversion, nullptr)) {}

    SamplerYcbcrConversion &SamplerYcbcrConversion::operator=(SamplerYcbcrConversion &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mSamplerYcbcrConversion = std::exchange(other.mSamplerYcbcrConversion, nullptr);
        }
        return *this;
    }

    const vk::SamplerYcbcrConversion &SamplerYcbcrConversion::getSamplerYcbcrConversion() const {
        return mSamplerYcbcrConversion;
    }

} // vklite
//
// Created by leixing on 2025/3/15.
//

#include "HardwareBufferYcbcrConversion.h"
#include "vklite/util/VkCheck.h"

namespace vklite {

    HardwareBufferYcbcrConversion::HardwareBufferYcbcrConversion(vk::Device device, vk::SamplerYcbcrConversion samplerYcbcrConversion)
            : mDevice(device), mSamplerYcbcrConversion(samplerYcbcrConversion) {}

    HardwareBufferYcbcrConversion::~HardwareBufferYcbcrConversion() {
        // device.destroySamplerYcbcrConversion(mConversion); // link error
        vkDestroySamplerYcbcrConversion(mDevice, mSamplerYcbcrConversion, nullptr);
    }

    HardwareBufferYcbcrConversion::HardwareBufferYcbcrConversion(HardwareBufferYcbcrConversion &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mSamplerYcbcrConversion(std::exchange(other.mSamplerYcbcrConversion, nullptr)) {}

    HardwareBufferYcbcrConversion &HardwareBufferYcbcrConversion::operator=(HardwareBufferYcbcrConversion &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mSamplerYcbcrConversion = std::exchange(other.mSamplerYcbcrConversion, nullptr);
        }
        return *this;
    }

    const vk::SamplerYcbcrConversion &HardwareBufferYcbcrConversion::getSamplerYcbcrConversion() const {
        return mSamplerYcbcrConversion;
    }

} // vklite
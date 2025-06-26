//
// Created by leixing on 2025/1/7.
//

#include "Sampler.h"

#include <utility>

#include "vklite/util/VulkanUtil.h"

namespace vklite {

    Sampler::Sampler(vk::Device device, vk::Sampler sampler)
            : mDevice(device), mSampler(sampler) {}

    Sampler::~Sampler() {
        if (mDevice != nullptr && mSampler != nullptr) {
            mDevice.destroy(mSampler);
            mDevice = nullptr;
            mSampler = nullptr;
        }
    }

    Sampler::Sampler(Sampler &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mSampler(std::exchange(other.mSampler, nullptr)) {}

    Sampler &Sampler::operator=(Sampler &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mSampler = std::exchange(other.mSampler, nullptr);
        }
        return *this;
    }

    const vk::Sampler &Sampler::getVkSampler() const {
        return mSampler;
    }

} // vklite
//
// Created by leixing on 2025/6/7.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/sampler/Sampler.h"
#include "vklite/platform/android/conversion/HardwareBufferYcbcrConversion.h"

namespace vklite {

    class CombinedHardwareBufferSampler {
    private:
        Sampler mSampler;
        HardwareBufferYcbcrConversion mConversion;

    public:
        CombinedHardwareBufferSampler(Sampler &&sampler, HardwareBufferYcbcrConversion &&conversion);

        ~CombinedHardwareBufferSampler();

        CombinedHardwareBufferSampler(const CombinedHardwareBufferSampler &other) = delete;

        CombinedHardwareBufferSampler &operator=(const CombinedHardwareBufferSampler &other) = delete;

        CombinedHardwareBufferSampler(CombinedHardwareBufferSampler &&other) noexcept;

        CombinedHardwareBufferSampler &operator=(CombinedHardwareBufferSampler &&other) noexcept;

        const Sampler &getSampler() const;

        const vk::Sampler &getVkSampler() const;

        const HardwareBufferYcbcrConversion &getConversion() const;
    };

} // vklite

//
// Created by leixing on 2025/6/7.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/platform/android/conversion/SamplerYcbcrConversion.h"
#include "vklite/image/Image.h"
#include "vklite/device_memory/DeviceMemory.h"
#include "vklite/image_view/ImageView.h"
#include "vklite/sampler/Sampler.h"

namespace vklite {

    class CombinedHardwareBufferSampler {
    private:
        SamplerYcbcrConversion mConversion;
        Sampler mSampler;

    public:
        explicit CombinedHardwareBufferSampler(SamplerYcbcrConversion &&conversion, Sampler &&sampler);

        ~CombinedHardwareBufferSampler();

        CombinedHardwareBufferSampler(const CombinedHardwareBufferSampler &other) = delete;

        CombinedHardwareBufferSampler &operator=(const CombinedHardwareBufferSampler &other) = delete;

        CombinedHardwareBufferSampler(CombinedHardwareBufferSampler &&other) noexcept;

        CombinedHardwareBufferSampler &operator=(CombinedHardwareBufferSampler &&other) noexcept;


        const SamplerYcbcrConversion &getConversion() const;

        const Sampler &getSampler() const;

        const vk::Sampler &getVkSampler() const;

    };

} // vklite

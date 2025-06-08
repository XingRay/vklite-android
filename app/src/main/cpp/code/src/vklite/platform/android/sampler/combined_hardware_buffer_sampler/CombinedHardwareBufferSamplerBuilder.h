//
// Created by leixing on 2025/6/7.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/platform/android/sampler/HardwareBufferSamplerBuilder.h"
#include "vklite/platform/android/conversion/HardwareBufferYcbcrConversionBuilder.h"
#include "vklite/platform/android/sampler/combined_hardware_buffer_sampler/CombinedHardwareBufferSampler.h"

namespace vklite {

    class CombinedHardwareBufferSamplerBuilder {
    private:
        vk::Device mDevice;
        HardwareBufferSamplerBuilder mHardwareBufferSamplerBuilder;
        HardwareBufferYcbcrConversionBuilder mHardwareBufferYcbcrConversionBuilder;

    public:
        CombinedHardwareBufferSamplerBuilder();

        ~CombinedHardwareBufferSamplerBuilder();

        CombinedHardwareBufferSamplerBuilder &device(vk::Device device);

        CombinedHardwareBufferSamplerBuilder &formatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties);

        CombinedHardwareBufferSampler build();

        std::unique_ptr<CombinedHardwareBufferSampler> buildUnique();
    };

} // vklite

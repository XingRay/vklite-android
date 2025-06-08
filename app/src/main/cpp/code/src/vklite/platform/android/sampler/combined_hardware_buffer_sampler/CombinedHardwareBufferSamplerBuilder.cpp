//
// Created by leixing on 2025/6/7.
//

#include "CombinedHardwareBufferSamplerBuilder.h"

namespace vklite {

    CombinedHardwareBufferSamplerBuilder::CombinedHardwareBufferSamplerBuilder() = default;

    CombinedHardwareBufferSamplerBuilder::~CombinedHardwareBufferSamplerBuilder() = default;

    CombinedHardwareBufferSamplerBuilder &CombinedHardwareBufferSamplerBuilder::device(vk::Device device) {
        mDevice = device;
        mHardwareBufferSamplerBuilder.device(device);
        mHardwareBufferYcbcrConversionBuilder.device(device);
        return *this;
    }

    CombinedHardwareBufferSamplerBuilder &CombinedHardwareBufferSamplerBuilder::formatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties) {
        mHardwareBufferYcbcrConversionBuilder.formatProperties(formatProperties);
        return *this;
    }

    CombinedHardwareBufferSampler CombinedHardwareBufferSamplerBuilder::build() {
        HardwareBufferYcbcrConversion conversion = mHardwareBufferYcbcrConversionBuilder.build();
        Sampler sampler = mHardwareBufferSamplerBuilder
                .ycbcrConversion(conversion.getSamplerYcbcrConversion())
                .build();
        return CombinedHardwareBufferSampler(std::move(sampler), std::move(conversion));
    }

    std::unique_ptr<CombinedHardwareBufferSampler> CombinedHardwareBufferSamplerBuilder::buildUnique() {
        return std::make_unique<CombinedHardwareBufferSampler>(build());
    }

} // vklite
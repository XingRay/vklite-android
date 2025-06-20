//
// Created by leixing on 2025/6/7.
//

#include "CombinedHardwareBufferSamplerBuilder.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/platform/android/api/VulkanAndroidApi.h"

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
        if (mDevice == nullptr) {
            throw std::runtime_error("CombinedHardwareBufferSamplerBuilder::build(): mDevice == nullptr");
        }
        SamplerYcbcrConversion conversion = mHardwareBufferYcbcrConversionBuilder.build();

        Sampler sampler = mHardwareBufferSamplerBuilder
                .ycbcrConversion(conversion.getSamplerYcbcrConversion())
                .build();

        return CombinedHardwareBufferSampler{
                std::move(conversion),
                std::move(sampler)
        };
    }

    std::unique_ptr<CombinedHardwareBufferSampler> CombinedHardwareBufferSamplerBuilder::buildUnique() {
        return std::make_unique<CombinedHardwareBufferSampler>(build());
    }

} // vklite
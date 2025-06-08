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
        SamplerYcbcrConversion conversion = mHardwareBufferYcbcrConversionBuilder.build();
        Image image = mHardwareBufferImageBuilder.build();
        DeviceMemory deviceMemory = mHardwareBufferDeviceMemoryBuilder.build();
        ImageView imageView = mHardwareBufferImageViewBuilder.build();
        Sampler sampler = mHardwareBufferSamplerBuilder
                .ycbcrConversion(conversion.getSamplerYcbcrConversion())
                .build();

//        return CombinedHardwareBufferSampler(std::move(conversion),
//                                             std::move(image),
//                                             std::move(deviceMemory),
//                                             std::move(imageView),
//                                             std::move(sampler));

        return {
                std::move(conversion),
                std::move(image),
                std::move(deviceMemory),
                std::move(imageView),
                std::move(sampler)
        };
    }

    std::unique_ptr<CombinedHardwareBufferSampler> CombinedHardwareBufferSamplerBuilder::buildUnique() {
        return std::make_unique<CombinedHardwareBufferSampler>(build());
    }

} // vklite
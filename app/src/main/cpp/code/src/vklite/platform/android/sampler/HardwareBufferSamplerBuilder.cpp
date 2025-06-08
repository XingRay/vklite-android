//
// Created by leixing on 2025/4/15.
//

#include "HardwareBufferSamplerBuilder.h"

namespace vklite {

    HardwareBufferSamplerBuilder::HardwareBufferSamplerBuilder() {
        mSamplerBuilder
                .filter(vk::Filter::eNearest)
                .mipmapMode(vk::SamplerMipmapMode::eNearest)
                .addressMode(vk::SamplerAddressMode::eClampToEdge)
                .mipLodBias(0.0f)
                .anisotropyEnable(false)
                .maxAnisotropy(1.0f)
                .compareEnable(false)
                .compareOp(vk::CompareOp::eNever)
                .lod(0.0f, 0.0f)
                .borderColor(vk::BorderColor::eFloatOpaqueWhite)
                .unnormalizedCoordinates(false)
                .next(&mConversionInfo);
    }

    HardwareBufferSamplerBuilder::~HardwareBufferSamplerBuilder() = default;

    HardwareBufferSamplerBuilder &HardwareBufferSamplerBuilder::device(vk::Device device) {
        mDevice = device;
        mSamplerBuilder.device(device);
        return *this;
    }

    HardwareBufferSamplerBuilder &HardwareBufferSamplerBuilder::ycbcrConversion(vk::SamplerYcbcrConversion ycbcrConversion) {
        mConversionInfo.setConversion(ycbcrConversion);
        return *this;
    }

    Sampler HardwareBufferSamplerBuilder::build() {
        return mSamplerBuilder.build();
    }

    std::unique_ptr<Sampler> HardwareBufferSamplerBuilder::buildUnique() {
        return std::make_unique<Sampler>(build());
    }

} // vklite
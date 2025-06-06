//
// Created by leixing on 2025/4/11.
//

#include "SamplerBuilder.h"
#include "vklite/sampler/Sampler.h"
#include "vklite/Log.h"

namespace vklite {

    SamplerBuilder::SamplerBuilder() {
        mSamplerCreateInfo
                .setMagFilter(vk::Filter::eLinear)
                .setMinFilter(vk::Filter::eLinear)
                .setAddressModeU(vk::SamplerAddressMode::eRepeat)
                .setAddressModeV(vk::SamplerAddressMode::eRepeat)
                .setAddressModeW(vk::SamplerAddressMode::eRepeat)
                .setAnisotropyEnable(vk::True)
                .setMaxAnisotropy(1.0f)
                .setBorderColor(vk::BorderColor::eIntOpaqueBlack)
                        // 是否使用不归一化的坐标(x:[0~width], y:[0~height])
                        // 设置为 false 就是使用归一化坐标 (x:[0~1.0], y:[0~1.0])
                .setUnnormalizedCoordinates(vk::False)
                .setCompareEnable(vk::False)
                .setCompareOp(vk::CompareOp::eAlways)
                .setMipmapMode(vk::SamplerMipmapMode::eLinear)
                .setMipLodBias(0.0f)
                .setMinLod(0.0f)
                .setMaxLod(0.0f);
    }

    SamplerBuilder::~SamplerBuilder() = default;

    SamplerBuilder &SamplerBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    SamplerBuilder &SamplerBuilder::maxLoad(float maxLoad) {
        mSamplerCreateInfo.setMaxLod(maxLoad);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::maxAnisotropy(float maxAnisotropy) {
        mSamplerCreateInfo.setMaxAnisotropy(maxAnisotropy);
        return *this;
    }

    Sampler SamplerBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("SamplerBuilder::build(): mDevice == nullptr");
        }

        vk::Sampler sampler = mDevice.createSampler(mSamplerCreateInfo);
        return {mDevice, sampler};
    }

    std::unique_ptr<Sampler> SamplerBuilder::buildUnique() {
        return std::make_unique<Sampler>(build());
    }

} // vklite
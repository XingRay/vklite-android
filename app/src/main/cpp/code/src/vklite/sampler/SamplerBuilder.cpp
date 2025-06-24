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

    SamplerBuilder &SamplerBuilder::maxAnisotropy(float maxAnisotropy) {
        mSamplerCreateInfo.setMaxAnisotropy(maxAnisotropy);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::magFilter(vk::Filter filter) {
        mSamplerCreateInfo.setMagFilter(filter);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::minFilter(vk::Filter filter) {
        mSamplerCreateInfo.setMinFilter(filter);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::addressModeU(vk::SamplerAddressMode mode) {
        mSamplerCreateInfo.setAddressModeU(mode);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::addressModeV(vk::SamplerAddressMode mode) {
        mSamplerCreateInfo.setAddressModeV(mode);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::addressModeW(vk::SamplerAddressMode mode) {
        mSamplerCreateInfo.setAddressModeW(mode);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::anisotropyEnable(vk::Bool32 enable) {
        mSamplerCreateInfo.setAnisotropyEnable(enable);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::borderColor(vk::BorderColor borderColor) {
        mSamplerCreateInfo.setBorderColor(borderColor);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::unnormalizedCoordinates(vk::Bool32 unnormalizedCoordinates) {
        mSamplerCreateInfo.setUnnormalizedCoordinates(unnormalizedCoordinates);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::compareEnable(vk::Bool32 enable) {
        mSamplerCreateInfo.setCompareEnable(enable);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::compareOp(vk::CompareOp op) {
        mSamplerCreateInfo.setCompareOp(op);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::mipmapMode(vk::SamplerMipmapMode mode) {
        mSamplerCreateInfo.setMipmapMode(mode);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::mipLodBias(float bias) {
        mSamplerCreateInfo.setMipLodBias(bias);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::minLod(float minLod) {
        mSamplerCreateInfo.setMinLod(minLod);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::maxLod(float maxLod) {
        mSamplerCreateInfo.setMaxLod(maxLod);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::next(const void *next) {
        mSamplerCreateInfo.setPNext(next);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::filter(vk::Filter filter) {
        magFilter(filter);
        minFilter(filter);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::addressMode(vk::SamplerAddressMode mode) {
        addressModeU(mode);
        addressModeV(mode);
        addressModeW(mode);
        return *this;
    }

    SamplerBuilder &SamplerBuilder::lod(float min, float max) {
        minLod(min);
        maxLod(max);
        return *this;
    }

    Sampler SamplerBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("SamplerBuilder::build(): mDevice == nullptr");
        }

        vk::Sampler sampler = mDevice.createSampler(mSamplerCreateInfo);
        return Sampler{mDevice, sampler};
    }

    std::vector<Sampler> SamplerBuilder::build(uint32_t count) {
        std::vector<Sampler> samplers;
        samplers.reserve(count);

        for (uint32_t i = 0; i < count; i++) {
            samplers.push_back(build());
        }

        return samplers;
    }

    std::unique_ptr<Sampler> SamplerBuilder::buildUnique() {
        return std::make_unique<Sampler>(build());
    }

} // vklite
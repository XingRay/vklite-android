//
// Created by leixing on 2025/1/26.
//

#include "HardwareBufferSampler.h"
#include "vklite/util/VkCheck.h"
#include "vklite/Log.h"

namespace vklite {

    HardwareBufferSampler::HardwareBufferSampler(const Device &device,
                                                 const HardwareBufferYcbcrConversion &samplerYcbcrConversion)
            : mDevice(device) {

        vk::Device vkDevice = device.getDevice();

        vk::SamplerCreateInfo samplerCreateInfo;
        vk::SamplerYcbcrConversionInfo conversionInfo;
        conversionInfo
                .setConversion(samplerYcbcrConversion.getSamplerYcbcrConversion());

        samplerCreateInfo
                .setPNext(&conversionInfo)
                .setMagFilter(vk::Filter::eNearest)
                .setMinFilter(vk::Filter::eNearest)
                .setMipmapMode(vk::SamplerMipmapMode::eNearest)
                .setAddressModeU(vk::SamplerAddressMode::eClampToEdge)
                .setAddressModeV(vk::SamplerAddressMode::eClampToEdge)
                .setAddressModeW(vk::SamplerAddressMode::eClampToEdge)
                .setMipLodBias(0.0f)
                .setAnisotropyEnable(false)
                .setMaxAnisotropy(1.0f)
                .setCompareEnable(false)
                .setCompareOp(vk::CompareOp::eNever)
                .setMinLod(0.0f)
                .setMaxLod(0.0f)
                .setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
                .setUnnormalizedCoordinates(false);

        mSampler = vkDevice.createSampler(samplerCreateInfo);
    }

    HardwareBufferSampler::~HardwareBufferSampler() {
        const vk::Device &vkDevice = mDevice.getDevice();

        vkDevice.destroySampler(mSampler);
    }

    HardwareBufferSampler::HardwareBufferSampler(HardwareBufferSampler &&other) noexcept
            : mDevice(other.mDevice),
              mSampler(std::exchange(other.mSampler, nullptr)) {}

    const vk::Sampler &HardwareBufferSampler::getSampler() const {
        return mSampler;
    }

} // vklite
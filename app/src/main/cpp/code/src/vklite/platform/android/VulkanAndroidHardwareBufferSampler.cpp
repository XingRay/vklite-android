//
// Created by leixing on 2025/1/26.
//

#include "VulkanAndroidHardwareBufferSampler.h"
#include "vklite/VkCheck.h"
#include "vklite/Log.h"

namespace vklite {

    VulkanAndroidHardwareBufferSampler::VulkanAndroidHardwareBufferSampler(const Device &device,
                                                                           const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion)
            : mDevice(device) {

        vk::Device vkDevice = device.getDevice();

        vk::SamplerCreateInfo samplerCreateInfo;
        vk::SamplerYcbcrConversionInfo conversionInfo;
        conversionInfo
                .setConversion(vulkanAndroidSamplerYcbcrConversion.getSamplerYcbcrConversion());

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

    VulkanAndroidHardwareBufferSampler::~VulkanAndroidHardwareBufferSampler() {
        vk::Device device = mDevice.getDevice();

        device.destroySampler(mSampler);
    }

    const vk::Sampler &VulkanAndroidHardwareBufferSampler::getSampler() const {
        return mSampler;
    }

} // vklite
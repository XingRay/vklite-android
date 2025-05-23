//
// Created by leixing on 2025/3/15.
//

#include "HardwareBufferYcbcrConversion.h"
#include "vklite/util/VkCheck.h"

namespace vklite {

    HardwareBufferYcbcrConversion::HardwareBufferYcbcrConversion(const Device &device,
                                                                 const vk::AndroidHardwareBufferFormatPropertiesANDROID &formatInfo)
            : mDevice(device) {

        vk::ExternalFormatANDROID externalFormat;
        if (formatInfo.format == vk::Format::eUndefined) {
            externalFormat.externalFormat = formatInfo.externalFormat;
        }

        vk::SamplerYcbcrModelConversion ycbcrModel;
        if (formatInfo.format == vk::Format::eUndefined) {
            ycbcrModel = formatInfo.suggestedYcbcrModel;
        } else {
            ycbcrModel = vk::SamplerYcbcrModelConversion::eYcbcr601;
        }

        vk::SamplerYcbcrConversionCreateInfo conversionCreateInfo;
        conversionCreateInfo
                .setPNext(&externalFormat)
                .setYcbcrModel(ycbcrModel)
                .setFormat(formatInfo.format)
                .setYcbcrRange(formatInfo.suggestedYcbcrRange)
                .setComponents(formatInfo.samplerYcbcrConversionComponents)
                .setXChromaOffset(formatInfo.suggestedXChromaOffset)
                .setYChromaOffset(formatInfo.suggestedYChromaOffset)
                .setChromaFilter(vk::Filter::eNearest)
                .setForceExplicitReconstruction(false);

        CALL_VK(vkCreateSamplerYcbcrConversion(device.getDevice(), reinterpret_cast<VkSamplerYcbcrConversionCreateInfo *>(&conversionCreateInfo), nullptr,
                                               reinterpret_cast<VkSamplerYcbcrConversion *>(&mSamplerYcbcrConversion)));

    }

    HardwareBufferYcbcrConversion::~HardwareBufferYcbcrConversion() {
        const vk::Device &device = mDevice.getDevice();

        // device.destroySamplerYcbcrConversion(mConversion); // link error
        vkDestroySamplerYcbcrConversion(device, mSamplerYcbcrConversion, nullptr);
    }

    HardwareBufferYcbcrConversion::HardwareBufferYcbcrConversion(HardwareBufferYcbcrConversion &&other) noexcept
            : mDevice(other.mDevice),
              mSamplerYcbcrConversion(std::exchange(other.mSamplerYcbcrConversion, nullptr)) {}

    const vk::SamplerYcbcrConversion &HardwareBufferYcbcrConversion::getSamplerYcbcrConversion() const {
        return mSamplerYcbcrConversion;
    }
} // vklite
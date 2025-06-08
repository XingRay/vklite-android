//
// Created by leixing on 2025/6/7.
//

#include "HardwareBufferYcbcrConversionBuilder.h"
#include "vklite/util/VkCheck.h"

namespace vklite {

    HardwareBufferYcbcrConversionBuilder::HardwareBufferYcbcrConversionBuilder() {
        mConversionCreateInfo
                .setPNext(&mExternalFormat)
                .setChromaFilter(vk::Filter::eNearest)
                .setForceExplicitReconstruction(false);
    };

    HardwareBufferYcbcrConversionBuilder::~HardwareBufferYcbcrConversionBuilder() = default;

    HardwareBufferYcbcrConversionBuilder &HardwareBufferYcbcrConversionBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    HardwareBufferYcbcrConversionBuilder &HardwareBufferYcbcrConversionBuilder::formatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties) {
        if (formatProperties.format == vk::Format::eUndefined) {
            mExternalFormat.externalFormat = formatProperties.externalFormat;
        }

        if (formatProperties.format == vk::Format::eUndefined) {
            mConversionCreateInfo.setYcbcrModel(formatProperties.suggestedYcbcrModel);
        } else {
            mConversionCreateInfo.setYcbcrModel(vk::SamplerYcbcrModelConversion::eYcbcr601);
        }

        mConversionCreateInfo
                .setFormat(formatProperties.format)
                .setYcbcrRange(formatProperties.suggestedYcbcrRange)
                .setComponents(formatProperties.samplerYcbcrConversionComponents)
                .setXChromaOffset(formatProperties.suggestedXChromaOffset)
                .setYChromaOffset(formatProperties.suggestedYChromaOffset);

        return *this;
    }

    HardwareBufferYcbcrConversion HardwareBufferYcbcrConversionBuilder::build() {
        vk::SamplerYcbcrConversion samplerYcbcrConversion;
        CALL_VK(vkCreateSamplerYcbcrConversion(mDevice, reinterpret_cast<VkSamplerYcbcrConversionCreateInfo *>(&mConversionCreateInfo), nullptr,
                                               reinterpret_cast<VkSamplerYcbcrConversion *>(&samplerYcbcrConversion)));
        return HardwareBufferYcbcrConversion(mDevice, samplerYcbcrConversion);
    }

    std::unique_ptr<HardwareBufferYcbcrConversion> HardwareBufferYcbcrConversionBuilder::buildUnique() {
        return std::make_unique<HardwareBufferYcbcrConversion>(build());
    }

} // vklite
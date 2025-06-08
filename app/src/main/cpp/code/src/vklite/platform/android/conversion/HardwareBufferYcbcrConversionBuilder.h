//
// Created by leixing on 2025/6/7.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/platform/android/conversion/HardwareBufferYcbcrConversion.h"

namespace vklite {

    class HardwareBufferYcbcrConversionBuilder {
    private:
        vk::Device mDevice;

        vk::ExternalFormatANDROID mExternalFormat;
        vk::SamplerYcbcrConversionCreateInfo mConversionCreateInfo;

    public:
        HardwareBufferYcbcrConversionBuilder();

        ~HardwareBufferYcbcrConversionBuilder();

        HardwareBufferYcbcrConversionBuilder &device(vk::Device device);

        HardwareBufferYcbcrConversionBuilder &formatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties);

        HardwareBufferYcbcrConversion build();

        std::unique_ptr<HardwareBufferYcbcrConversion> buildUnique();
    };

} // vklite

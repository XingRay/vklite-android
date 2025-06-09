//
// Created by leixing on 2025/6/7.
//

#pragma once

#include <memory>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "vklite/platform/android/conversion/SamplerYcbcrConversion.h"

namespace vklite {

    class SamplerYcbcrConversionBuilder {
    private:
        vk::Device mDevice;

        vk::ExternalFormatANDROID mExternalFormat;
        vk::SamplerYcbcrConversionCreateInfo mConversionCreateInfo;

        std::optional<vk::AndroidHardwareBufferFormatPropertiesANDROID> mFormatProperties;

    public:
        SamplerYcbcrConversionBuilder();

        ~SamplerYcbcrConversionBuilder();

        SamplerYcbcrConversionBuilder &device(vk::Device device);

        SamplerYcbcrConversionBuilder &formatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties);

        SamplerYcbcrConversion build();

        std::unique_ptr<SamplerYcbcrConversion> buildUnique();
    };

} // vklite

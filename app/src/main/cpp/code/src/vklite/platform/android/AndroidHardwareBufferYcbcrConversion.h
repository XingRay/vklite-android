//
// Created by leixing on 2025/3/15.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "vklite/device/Device.h"

namespace vklite {

    class AndroidHardwareBufferYcbcrConversion {
    private:
        const Device& mDevice;

        vk::SamplerYcbcrConversion mSamplerYcbcrConversion;

    public:
        AndroidHardwareBufferYcbcrConversion(const Device &device,
                                             const vk::AndroidHardwareBufferFormatPropertiesANDROID &formatInfo);

        ~AndroidHardwareBufferYcbcrConversion();

        [[nodiscard]]
        const vk::SamplerYcbcrConversion& getSamplerYcbcrConversion()const;
    };

} // vklite

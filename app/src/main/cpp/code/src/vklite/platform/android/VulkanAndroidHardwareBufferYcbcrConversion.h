//
// Created by leixing on 2025/3/15.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "vklite/device/Device.h"

namespace vklite {

    class VulkanAndroidHardwareBufferYcbcrConversion {
    private:
        const Device& mVulkanDevice;

        vk::SamplerYcbcrConversion mSamplerYcbcrConversion;

    public:
        VulkanAndroidHardwareBufferYcbcrConversion(const Device &vulkanDevice,
                                                   const vk::AndroidHardwareBufferFormatPropertiesANDROID &formatInfo);

        ~VulkanAndroidHardwareBufferYcbcrConversion();

        [[nodiscard]]
        const vk::SamplerYcbcrConversion& getSamplerYcbcrConversion()const;
    };

} // vklite

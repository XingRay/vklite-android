//
// Created by leixing on 2025/3/15.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"

namespace vklite {

    class HardwareBufferYcbcrConversion {
    private:
        const Device &mDevice;

        vk::SamplerYcbcrConversion mSamplerYcbcrConversion;

    public:
        HardwareBufferYcbcrConversion(const Device &device,
                                      const vk::AndroidHardwareBufferFormatPropertiesANDROID &formatInfo);

        ~HardwareBufferYcbcrConversion();

        HardwareBufferYcbcrConversion(const HardwareBufferYcbcrConversion &other) = delete;

        HardwareBufferYcbcrConversion &operator=(const HardwareBufferYcbcrConversion &other) = delete;

        HardwareBufferYcbcrConversion(HardwareBufferYcbcrConversion &&other) noexcept;

        HardwareBufferYcbcrConversion &operator=(HardwareBufferYcbcrConversion &&other) noexcept = delete;

        [[nodiscard]]
        const vk::SamplerYcbcrConversion &getSamplerYcbcrConversion() const;
    };

} // vklite

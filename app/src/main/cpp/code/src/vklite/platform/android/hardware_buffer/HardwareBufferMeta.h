//
// Created by leixing on 2025-06-20.
//

#pragma once

#include <android/hardware_buffer.h>
#include <vulkan/vulkan.hpp>

namespace vklite {

    class HardwareBufferMeta {
    private:
        vk::AndroidHardwareBufferPropertiesANDROID mPropertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID mFormatInfo;
        AHardwareBuffer_Desc mHardwareBufferDescription;

    public:
        HardwareBufferMeta(vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo,
                           vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo,
                           AHardwareBuffer_Desc hardwareBufferDescription);

        ~HardwareBufferMeta();

        HardwareBufferMeta(const HardwareBufferMeta &other);

        HardwareBufferMeta &operator=(const HardwareBufferMeta &other);

        HardwareBufferMeta(HardwareBufferMeta &&other) noexcept;

        HardwareBufferMeta &operator=(HardwareBufferMeta &&other) noexcept;

        [[nodiscard]]
        const vk::AndroidHardwareBufferPropertiesANDROID &getPropertiesInfo() const;

        [[nodiscard]]
        const vk::AndroidHardwareBufferFormatPropertiesANDROID &getFormatInfo() const;

        [[nodiscard]]
        const AHardwareBuffer_Desc &getHardwareBufferDescription() const;

        [[nodiscard]]
        uint32_t getDataSize() const;
    };

} // vklite

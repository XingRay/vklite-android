//
// Created by leixing on 2025/3/16.
//

#pragma once

#include <android/hardware_buffer.h>

#include "vklite/device/Device.h"

namespace vklite {

    class HardwareBuffer {
    private:
        AHardwareBuffer *mHardwareBuffer;
        vk::AndroidHardwareBufferPropertiesANDROID mPropertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID mFormatInfo;
        AHardwareBuffer_Desc mHardwareBufferDescription;

    public:
        HardwareBuffer(AHardwareBuffer *hardwareBuffer,
                       vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo,
                       vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo,
                       AHardwareBuffer_Desc hardwareBufferDescription);

        ~HardwareBuffer();

        [[nodiscard]]
        vk::AndroidHardwareBufferPropertiesANDROID getProperties() const;

        [[nodiscard]]
        vk::AndroidHardwareBufferFormatPropertiesANDROID getFormatProperties() const;

        [[nodiscard]]
        AHardwareBuffer_Desc getAndroidHardwareBufferDescription() const;

        [[nodiscard]]
        uint32_t getDataSize() const;

        [[nodiscard]]
        AHardwareBuffer *getHardwareBuffer() const;

    public://static
        static HardwareBuffer build(const Device &device, AHardwareBuffer *hardwareBuffer);
    };

} // vklite
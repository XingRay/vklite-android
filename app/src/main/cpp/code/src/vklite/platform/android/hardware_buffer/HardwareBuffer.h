//
// Created by leixing on 2025/3/16.
//

#pragma once

#include <android/hardware_buffer.h>
#include <vulkan/vulkan.hpp>

#include "vklite/platform/android/hardware_buffer/HardwareBufferMeta.h"

namespace vklite {

    class HardwareBuffer {
    private:
        AHardwareBuffer *mHardwareBuffer;
        HardwareBufferMeta mMeta;

    public:
        explicit HardwareBuffer(AHardwareBuffer *hardwareBuffer,
                                HardwareBufferMeta meta);

        ~HardwareBuffer();

        HardwareBuffer(const HardwareBuffer &other);

        HardwareBuffer &operator=(const HardwareBuffer &other);

        HardwareBuffer(HardwareBuffer &&other) noexcept;

        HardwareBuffer &operator=(HardwareBuffer &&other) noexcept;

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

    };

} // vklite
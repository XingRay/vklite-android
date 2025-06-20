//
// Created by leixing on 2025/3/16.
//

#pragma once

#include <android/hardware_buffer.h>
#include <vulkan/vulkan.hpp>
#include "vklite/platform/android/hardware_buffer/HardwareBuffer.h"

namespace vklite {

    class HardwareBufferBuilder {
    private:
        vk::Device mDevice;
        AHardwareBuffer *mHardwareBuffer;

    public:
        HardwareBufferBuilder();

        ~HardwareBufferBuilder();

        HardwareBufferBuilder &device(vk::Device device);

        HardwareBufferBuilder &hardwareBuffer(AHardwareBuffer *hardwareBuffer);

        HardwareBuffer build();

        std::unique_ptr<HardwareBuffer> buildUnique();
    };

} // vklite
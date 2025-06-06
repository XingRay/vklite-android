//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include "vklite/platform/android/sampler/HardwareBufferYcbcrConversion.h"
#include "vklite/platform/android/hardware_buffer/HardwareBuffer.h"
#include "vklite/image/Image.h"
#include "vklite/physical_device/PhysicalDevice.h"

namespace vklite {

    class HardwareBufferImageBuilder {
    private:
        const HardwareBufferYcbcrConversion &mAndroidSamplerYcbcrConversion;
        const HardwareBuffer &mAndroidHardwareBuffer;

    public:
        HardwareBufferImageBuilder(const HardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion,
                                   const HardwareBuffer &androidHardwareBuffer);

        ~HardwareBufferImageBuilder();

        std::unique_ptr<Image> build(const PhysicalDevice &physicalDevice, const Device &device, uint32_t width, uint32_t height, vk::Format format);
    };

} // vklite

//
// Created by leixing on 2025/4/15.
//

#include "HardwareBufferImageBuilder.h"
#include "HardwareBufferImage.h"

namespace vklite {

    HardwareBufferImageBuilder::HardwareBufferImageBuilder(const HardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion,
                                                           const HardwareBuffer &androidHardwareBuffer)
            : mAndroidSamplerYcbcrConversion(vulkanAndroidSamplerYcbcrConversion),
              mAndroidHardwareBuffer(androidHardwareBuffer) {}

    HardwareBufferImageBuilder::~HardwareBufferImageBuilder() = default;

    std::unique_ptr<Image> HardwareBufferImageBuilder::build(const PhysicalDevice &physicalDevice, const Device &device, uint32_t width, uint32_t height, vk::Format format) {
//        return std::make_unique<HardwareBufferImage>(physicalDevice, device, mAndroidHardwareBuffer, mAndroidSamplerYcbcrConversion);
        return nullptr;
    }

} // vklite
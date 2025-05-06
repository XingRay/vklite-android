//
// Created by leixing on 2025/4/15.
//

#include "VulkanAndroidHardwareBufferImageBuilder.h"
#include "VulkanAndroidHardwareBufferImage.h"

namespace vklite {

    VulkanAndroidHardwareBufferImageBuilder::VulkanAndroidHardwareBufferImageBuilder(const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion,
                                                                                     const AndroidHardwareBuffer &androidHardwareBuffer)
            : mAndroidSamplerYcbcrConversion(vulkanAndroidSamplerYcbcrConversion),
              mAndroidHardwareBuffer(androidHardwareBuffer) {}

    VulkanAndroidHardwareBufferImageBuilder::~VulkanAndroidHardwareBufferImageBuilder() = default;

    std::unique_ptr<ImageInterface> VulkanAndroidHardwareBufferImageBuilder::build(const Device &device, uint32_t width, uint32_t height, vk::Format format) {
        return std::make_unique<VulkanAndroidHardwareBufferImage>(device, mAndroidHardwareBuffer, mAndroidSamplerYcbcrConversion);
    }

} // vklite
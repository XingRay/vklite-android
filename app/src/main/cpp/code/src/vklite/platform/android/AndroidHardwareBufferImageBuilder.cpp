//
// Created by leixing on 2025/4/15.
//

#include "AndroidHardwareBufferImageBuilder.h"
#include "AndroidHardwareBufferImage.h"

namespace vklite {

    AndroidHardwareBufferImageBuilder::AndroidHardwareBufferImageBuilder(const AndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion,
                                                                         const AndroidHardwareBuffer &androidHardwareBuffer)
            : mAndroidSamplerYcbcrConversion(vulkanAndroidSamplerYcbcrConversion),
              mAndroidHardwareBuffer(androidHardwareBuffer) {}

    AndroidHardwareBufferImageBuilder::~AndroidHardwareBufferImageBuilder() = default;

    std::unique_ptr<ImageInterface> AndroidHardwareBufferImageBuilder::build(const Device &device, uint32_t width, uint32_t height, vk::Format format) {
        return std::make_unique<AndroidHardwareBufferImage>(device, mAndroidHardwareBuffer, mAndroidSamplerYcbcrConversion);
    }

} // vklite
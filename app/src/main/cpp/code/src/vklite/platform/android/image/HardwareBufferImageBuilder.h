//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include "vklite/image/ImageBuilderInterface.h"
#include "vklite/platform/android/sampler/HardwareBufferYcbcrConversion.h"
#include "vklite/platform/android/hardware_buffer/HardwareBuffer.h"

namespace vklite {

    class HardwareBufferImageBuilder : public ImageBuilderInterface {
    private:
        const HardwareBufferYcbcrConversion &mAndroidSamplerYcbcrConversion;
        const HardwareBuffer &mAndroidHardwareBuffer;

    public:
        HardwareBufferImageBuilder(const HardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion,
                                   const HardwareBuffer &androidHardwareBuffer);

        ~HardwareBufferImageBuilder() override;

        std::unique_ptr<ImageInterface> build(const Device &device, uint32_t width, uint32_t height, vk::Format format) override;
    };

} // vklite

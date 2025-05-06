//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include "vklite/image/ImageBuilderInterface.h"
#include "AndroidHardwareBufferYcbcrConversion.h"
#include "AndroidHardwareBuffer.h"

namespace vklite {

    class AndroidHardwareBufferImageBuilder : public ImageBuilderInterface {
    private:
        const AndroidHardwareBufferYcbcrConversion &mAndroidSamplerYcbcrConversion;
        const AndroidHardwareBuffer &mAndroidHardwareBuffer;

    public:
        AndroidHardwareBufferImageBuilder(const AndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion,
                                          const AndroidHardwareBuffer &androidHardwareBuffer);

        ~AndroidHardwareBufferImageBuilder() override;

        std::unique_ptr<ImageInterface> build(const Device &device, uint32_t width, uint32_t height, vk::Format format) override;
    };

} // vklite

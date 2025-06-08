//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>
#include <android/hardware_buffer.h>

#include "vklite/image/ImageBuilder.h"
#include "vklite/image/Image.h"


namespace vklite {

    class HardwareBufferImageBuilder {
    private:
        vk::Device mDevice;

        vk::ExternalMemoryImageCreateInfo mExternalMemoryImageCreateInfo;
        vk::ExternalFormatANDROID mExternalFormat;
        ImageBuilder mImageBuilder;

    public:
        HardwareBufferImageBuilder();

        ~HardwareBufferImageBuilder();

        HardwareBufferImageBuilder &device(vk::Device device);

        HardwareBufferImageBuilder &externalFormat(uint64_t externalFormat);

        HardwareBufferImageBuilder &formatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties);

        HardwareBufferImageBuilder &hardwareBufferDescription(AHardwareBuffer_Desc hardwareBufferDescription);

        [[nodiscard]]
        Image build();

        [[nodiscard]]
        std::unique_ptr<Image> buildUnique();
    };

} // vklite

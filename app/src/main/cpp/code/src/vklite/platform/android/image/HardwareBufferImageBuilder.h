//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>
#include <optional>

#include <vulkan/vulkan.hpp>
#include <android/hardware_buffer.h>

#include "vklite/platform/android/image/HardwareBufferImage.h"
#include "vklite/platform/android/image/HardwareBufferImageMeta.h"

namespace vklite {

    class HardwareBufferImageBuilder {
    private:
        vk::Device mDevice;

        vk::ImageCreateInfo mImageCreateInfo;
        vk::ExternalMemoryImageCreateInfo mExternalMemoryImageCreateInfo;
        vk::ExternalFormatANDROID mExternalFormat;


        std::vector<uint32_t> mQueueFamilyIndices;

        // required params
        std::optional<vk::AndroidHardwareBufferFormatPropertiesANDROID> mHardwareBufferFormatProperties;
        std::optional<AHardwareBuffer_Desc> mHardwareBufferDescription;

    public:
        HardwareBufferImageBuilder();

        ~HardwareBufferImageBuilder();

        // setter
        HardwareBufferImageBuilder &device(vk::Device device);

        HardwareBufferImageBuilder &externalFormat(uint64_t externalFormat);

        HardwareBufferImageBuilder &format(vk::Format format);

        HardwareBufferImageBuilder &hardwareBufferFormatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties);

        HardwareBufferImageBuilder &hardwareBufferDescription(AHardwareBuffer_Desc hardwareBufferDescription);


        [[nodiscard]]
        HardwareBufferImage build();

        [[nodiscard]]
        std::unique_ptr<HardwareBufferImage> buildUnique();
    };

} // vklite

//
// Created by leixing on 2025/6/8.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/platform/android/image_view/combined_hardware_buffer_image_view/CombinedHardwareBufferImageView.h"

#include "vklite/platform/android/image/HardwareBufferImageBuilder.h"
#include "vklite/platform/android/device_memory/HardwareBufferDeviceMemoryBuilder.h"
#include "vklite/platform/android/image_view/HardwareBufferImageViewBuilder.h"

#include "vklite/platform/android/hardware_buffer/HardwareBuffer.h"

namespace vklite {

    class CombinedHardwareBufferImageViewBuilder {
    private:
        vk::Device mDevice;
        HardwareBufferImageBuilder mHardwareBufferImageBuilder;
        HardwareBufferDeviceMemoryBuilder mHardwareBufferDeviceMemoryBuilder;
        HardwareBufferImageViewBuilder mHardwareBufferImageViewBuilder;

        // hardware buffer
        AHardwareBuffer *mHardwareBuffer;
        std::optional<AHardwareBuffer_Desc> mHardwareBufferDescription;
        std::optional<vk::AndroidHardwareBufferPropertiesANDROID> mHardwareBufferProperties;
        std::optional<vk::AndroidHardwareBufferFormatPropertiesANDROID> mHardwareBufferFormatProperties;

        // conversion
        std::optional<vk::SamplerYcbcrConversion> mSamplerYcbcrConversion;

        // bind memory
        vk::DeviceSize mMemoryOffset;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;


    public:
        CombinedHardwareBufferImageViewBuilder();

        ~CombinedHardwareBufferImageViewBuilder();

        CombinedHardwareBufferImageViewBuilder &device(vk::Device device);

        CombinedHardwareBufferImageViewBuilder &hardwareBufferDescription(AHardwareBuffer_Desc hardwareBufferDescription);

        CombinedHardwareBufferImageViewBuilder &hardwareBufferFormatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties);

        CombinedHardwareBufferImageViewBuilder &hardwareBufferProperties(vk::AndroidHardwareBufferPropertiesANDROID hardwareBufferProperties);

        CombinedHardwareBufferImageViewBuilder &hardwareBuffer(const HardwareBuffer &androidHardwareBuffer);

        CombinedHardwareBufferImageViewBuilder &hardwareBuffer(AHardwareBuffer *hardwareBuffer);

        CombinedHardwareBufferImageViewBuilder &memoryProperties(vk::PhysicalDeviceMemoryProperties memoryProperties);

        CombinedHardwareBufferImageViewBuilder &conversion(const vk::SamplerYcbcrConversion &conversion);


        CombinedHardwareBufferImageView build();

        std::unique_ptr<CombinedHardwareBufferImageView> buildUnique();
    };

} // vklite

//
// Created by leixing on 2025/6/7.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/platform/android/conversion/SamplerYcbcrConversionBuilder.h"
#include "vklite/platform/android/image/HardwareBufferImageBuilder.h"
#include "vklite/platform/android/device_memory/HardwareBufferDeviceMemoryBuilder.h"
#include "vklite/platform/android/image_view/HardwareBufferImageViewBuilder.h"
#include "vklite/platform/android/sampler/HardwareBufferSamplerBuilder.h"

#include "vklite/platform/android/sampler/combined_hardware_buffer_sampler/CombinedHardwareBufferSampler.h"

namespace vklite {

    class CombinedHardwareBufferSamplerBuilder {
    private:
        vk::Device mDevice;

        SamplerYcbcrConversionBuilder mHardwareBufferYcbcrConversionBuilder;
        HardwareBufferImageBuilder mHardwareBufferImageBuilder;
        HardwareBufferDeviceMemoryBuilder mHardwareBufferDeviceMemoryBuilder;
        HardwareBufferImageViewBuilder mHardwareBufferImageViewBuilder;
        HardwareBufferSamplerBuilder mHardwareBufferSamplerBuilder;

        // bind memory
        vk::DeviceSize mMemoryOffset;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;
        vk::MemoryPropertyFlags mMemoryPropertyFlags;// = vk::MemoryPropertyFlagBits::eDeviceLocal;

    public:
        CombinedHardwareBufferSamplerBuilder();

        ~CombinedHardwareBufferSamplerBuilder();

        CombinedHardwareBufferSamplerBuilder &device(vk::Device device);

        CombinedHardwareBufferSamplerBuilder &formatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties);

        CombinedHardwareBufferSampler build();

        std::unique_ptr<CombinedHardwareBufferSampler> buildUnique();
    };

} // vklite

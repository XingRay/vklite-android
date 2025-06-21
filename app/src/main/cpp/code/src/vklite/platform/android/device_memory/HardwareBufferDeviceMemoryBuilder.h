//
// Created by leixing on 2025/6/8.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/device_memory/DeviceMemoryBuilder.h"

namespace vklite {

    class HardwareBufferDeviceMemoryBuilder {
    private:
        vk::Device mDevice;
        DeviceMemoryBuilder mDeviceMemoryBuilder;

        vk::ImportAndroidHardwareBufferInfoANDROID mHardwareBufferInfo;
        vk::MemoryDedicatedAllocateInfo mMemoryDedicatedAllocateInfo;

        AHardwareBuffer *mHardwareBuffer;

    public:
        explicit HardwareBufferDeviceMemoryBuilder();

        ~HardwareBufferDeviceMemoryBuilder();

        HardwareBufferDeviceMemoryBuilder &device(vk::Device device);

        HardwareBufferDeviceMemoryBuilder &hardwareBuffer(AHardwareBuffer *hardwareBuffer);

        HardwareBufferDeviceMemoryBuilder &image(vk::Image image);

        HardwareBufferDeviceMemoryBuilder &allocationSize(vk::DeviceSize allocationSize);

        HardwareBufferDeviceMemoryBuilder &memoryTypeIndex(uint32_t memoryTypeIndex);

        DeviceMemory build();

        std::unique_ptr<DeviceMemory> buildUnique();
    };

} // vklite

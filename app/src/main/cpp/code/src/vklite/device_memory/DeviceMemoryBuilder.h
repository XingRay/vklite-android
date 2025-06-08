//
// Created by leixing on 2025/5/30.
//

#pragma once

#include <memory>

#include <vulkan//vulkan.hpp>

#include "vklite/device_memory/DeviceMemory.h"

namespace vklite {

    class DeviceMemoryBuilder {
    private:
        vk::Device mDevice;
        vk::MemoryAllocateInfo mMemoryAllocateInfo;

    public:

        DeviceMemoryBuilder &device(const vk::Device &device);

        DeviceMemoryBuilder &allocationSize(vk::DeviceSize allocationSize);

        DeviceMemoryBuilder &memoryTypeIndex(uint32_t memoryTypeIndex);

        DeviceMemoryBuilder &next(const void* next);

        DeviceMemory build();

        std::unique_ptr<DeviceMemory> buildUnique();

    };

} // vklite

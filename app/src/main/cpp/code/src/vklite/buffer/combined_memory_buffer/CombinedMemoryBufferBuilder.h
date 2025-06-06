//
// Created by leixing on 2025/5/31.
//

#pragma once

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBuffer.h"
#include "vklite/buffer/Buffer.h"
#include "vklite/buffer/BufferBuilder.h"
#include "vklite/device_memory/DeviceMemoryBuilder.h"

namespace vklite {

    class CombinedMemoryBufferBuilder {
    private:
        vk::Device mDevice;
        BufferBuilder mBufferBuilder;

        DeviceMemoryBuilder mDeviceMemoryBuilder;
        vk::DeviceSize mMemoryOffset;

        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;
        std::optional<vk::MemoryPropertyFlags> mMemoryPropertyFlags;

//        std::function<void(BufferBuilder &builder)> mBufferBuilderConfigure;
//        std::function<void(Buffer &buffer, DeviceMemoryBuilder &builder)> mDeviceMemoryBuilderConfigure;

    public:
        CombinedMemoryBufferBuilder();

        ~CombinedMemoryBufferBuilder();

        CombinedMemoryBufferBuilder &device(vk::Device device);

        CombinedMemoryBufferBuilder &size(vk::DeviceSize size);

        CombinedMemoryBufferBuilder &sharingMode(vk::SharingMode sharingMode);

        CombinedMemoryBufferBuilder &usage(vk::BufferUsageFlags usage);

        CombinedMemoryBufferBuilder &addUsage(vk::BufferUsageFlags usage);

        CombinedMemoryBufferBuilder &memoryPropertyFlags(vk::MemoryPropertyFlags memoryPropertyFlags);

        CombinedMemoryBufferBuilder &physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        CombinedMemoryBufferBuilder &memoryOffset(vk::DeviceSize memoryOffset);

//        CombinedMemoryBufferBuilder &configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
//                                                        vk::MemoryPropertyFlags memoryPropertyFlags);

//        CombinedMemoryBufferBuilder &bufferBuilder(std::function<void(BufferBuilder &builder)> &&configure);
//
//        CombinedMemoryBufferBuilder &deviceMemoryBuilder(std::function<void(Buffer &buffer, DeviceMemoryBuilder &builder)> &&configure);

        CombinedMemoryBuffer build();

        std::unique_ptr<CombinedMemoryBuffer> buildUnique();

        // preset
        CombinedMemoryBufferBuilder &asDefault();

        CombinedMemoryBufferBuilder &asDeviceLocal();

        CombinedMemoryBufferBuilder &asHostVisible();
    };

} // vklite
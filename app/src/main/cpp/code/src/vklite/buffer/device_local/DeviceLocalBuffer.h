//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"
#include "vklite/command/CommandPool.h"

namespace vklite {

    /**
     * buffer on gpu, and invisible on cpu.
     */
    class DeviceLocalBuffer {
    private:
        const Device &mDevice;

        vk::Buffer mBuffer;
        vk::DeviceSize mBufferSize;
        vk::DeviceMemory mDeviceMemory;

    public:
        DeviceLocalBuffer(const Device &device, vk::DeviceSize bufferSize, vk::BufferUsageFlags bufferUsageFlagBits);

        virtual  ~DeviceLocalBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        vk::DeviceSize getBufferSize() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        void recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer);

        void copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset);

        void copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer);
    };

} // vklite

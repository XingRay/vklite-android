//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"

namespace vklite {

    /**
     * buffer visible on cpu
     */
    class VulkanHostVisibleBuffer {
    private:
        const Device &mVulkanDevice;

        vk::Buffer mBuffer;
        vk::DeviceSize mBufferSize;
        vk::DeviceMemory mDeviceMemory;
        void *mMappedMemoryPointer;

    public:
        VulkanHostVisibleBuffer(const Device &vulkanDevice, vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsageFlagBits);

        virtual  ~VulkanHostVisibleBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceSize &getBufferSize() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void updateBuffer(const void *data, uint32_t size);
    };

} // vklite

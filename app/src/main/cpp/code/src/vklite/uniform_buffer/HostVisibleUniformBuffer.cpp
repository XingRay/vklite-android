//
// Created by leixing on 2025/3/13.
//

#include "HostVisibleUniformBuffer.h"

namespace vklite {
    HostVisibleUniformBuffer::HostVisibleUniformBuffer(const PhysicalDevice &physicalDevice, const Device &device, vk::DeviceSize bufferSize)
            : mUniformBuffer(physicalDevice, device, bufferSize, vk::BufferUsageFlagBits::eUniformBuffer) {}

    HostVisibleUniformBuffer::~HostVisibleUniformBuffer() = default;

    const vk::Buffer &HostVisibleUniformBuffer::getBuffer() const {
        return mUniformBuffer.getBuffer();
    }

    const vk::DeviceSize &HostVisibleUniformBuffer::getBufferSize() const {
        return mUniformBuffer.getBufferSize();
    }

    const vk::DeviceMemory &HostVisibleUniformBuffer::getDeviceMemory() const {
        return mUniformBuffer.getDeviceMemory();
    }

    void HostVisibleUniformBuffer::updateBuffer(const void *data, uint32_t size) {
        mUniformBuffer.updateBuffer(data, size);
    }
} // vklite
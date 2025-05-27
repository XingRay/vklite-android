//
// Created by leixing on 2025/3/13.
//

#include "HostVisibleVertexBuffer.h"

namespace vklite {
    HostVisibleVertexBuffer::HostVisibleVertexBuffer(const PhysicalDevice &physicalDevice,const Device &device, vk::DeviceSize bufferSize)
            : mVertexBuffer(physicalDevice, device, bufferSize, vk::BufferUsageFlagBits::eVertexBuffer) {

    }

    HostVisibleVertexBuffer::~HostVisibleVertexBuffer() = default;

    const vk::Buffer &HostVisibleVertexBuffer::getBuffer() const {
        return mVertexBuffer.getBuffer();
    }

    const vk::DeviceSize &HostVisibleVertexBuffer::getBufferSize() const {
        return mVertexBuffer.getBufferSize();
    }

    const vk::DeviceMemory &HostVisibleVertexBuffer::getDeviceMemory() const {
        return mVertexBuffer.getDeviceMemory();
    }

    void HostVisibleVertexBuffer::updateBuffer(const void *data, uint32_t size) {
        mVertexBuffer.updateBuffer(data, size);
    }

} // vklite
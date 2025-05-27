//
// Created by leixing on 2025/3/13.
//

#include "VertexBuffer.h"

namespace vklite {
    VertexBuffer::VertexBuffer(const PhysicalDevice &physicalDevice, const Device &device, vk::DeviceSize bufferSize)
            : mVertexBuffer(physicalDevice, device, bufferSize, vk::BufferUsageFlagBits::eVertexBuffer),
              mStagingBuffer(physicalDevice, device, bufferSize) {}

    VertexBuffer::~VertexBuffer() = default;

    const vk::Buffer &VertexBuffer::getBuffer() const {
        return mVertexBuffer.getBuffer();
    }

    const vk::DeviceMemory &VertexBuffer::getDeviceMemory() const {
        return mVertexBuffer.getDeviceMemory();
    }

    void VertexBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mVertexBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void VertexBuffer::update(const CommandPool &commandPool, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mVertexBuffer.copyFrom(commandPool, mStagingBuffer.getBuffer());
    }
} // vklite
//
// Created by leixing on 2025/3/13.
//

#include "IndexBuffer.h"

namespace vklite {

    IndexBuffer::IndexBuffer(const Device &device, vk::DeviceSize bufferSize)
            : mIndexBuffer(device, bufferSize, vk::BufferUsageFlagBits::eIndexBuffer),
              mStagingBuffer(device, bufferSize) {

    }

    IndexBuffer::~IndexBuffer() = default;

    [[nodiscard]]
    const vk::Buffer &IndexBuffer::getBuffer() const {
        return mIndexBuffer.getBuffer();
    }

    [[nodiscard]]
    const vk::DeviceMemory &IndexBuffer::getDeviceMemory() const {
        return mIndexBuffer.getDeviceMemory();
    }

    [[nodiscard]]
    uint32_t IndexBuffer::getIndicesCount() const {
        return mIndicesCount;
    }

    void IndexBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<uint32_t> &indices) {
        size_t size = indices.size() * sizeof(uint32_t);
        mIndicesCount = indices.size();

        mStagingBuffer.updateBuffer(indices.data(), size);
        mIndexBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void IndexBuffer::update(const CommandPool &vulkanCommandPool, const std::vector<uint32_t> &indices) {
        size_t size = indices.size() * sizeof(uint32_t);
        mIndicesCount = indices.size();

        mStagingBuffer.updateBuffer(indices.data(), size);
        mIndexBuffer.copyFrom(vulkanCommandPool, mStagingBuffer.getBuffer());
    }

} // vklite
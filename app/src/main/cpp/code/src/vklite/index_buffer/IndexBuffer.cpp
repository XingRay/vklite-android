//
// Created by leixing on 2025/3/13.
//

#include "IndexBuffer.h"

namespace vklite {

    IndexBuffer::IndexBuffer(const PhysicalDevice &physicalDevice, const Device &device, vk::DeviceSize bufferSize)
            : mIndexBuffer(physicalDevice, device, bufferSize, vk::BufferUsageFlagBits::eIndexBuffer),
              mIndexType(vk::IndexType::eUint32),
              mIndicesCount(0),
              mStagingBuffer(physicalDevice, device, bufferSize) {}

    IndexBuffer::~IndexBuffer() = default;

    const vk::Buffer &IndexBuffer::getBuffer() const {
        return mIndexBuffer.getBuffer();
    }

    vk::IndexType IndexBuffer::getIndexType() const {
        return mIndexType;
    }

    [[nodiscard]]
    const vk::DeviceMemory &IndexBuffer::getDeviceMemory() const {
        return mIndexBuffer.getDeviceMemory();
    }

    [[nodiscard]]
    uint32_t IndexBuffer::getIndicesCount() const {
        return mIndicesCount;
    }

    IndexBuffer &IndexBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<uint32_t> &indices) {
        size_t size = indices.size() * sizeof(uint32_t);
        mIndicesCount = indices.size();

        mStagingBuffer.updateBuffer(indices.data(), size);
        mIndexBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());

        return *this;
    }

    IndexBuffer &IndexBuffer::update(const CommandPool &commandPool, const std::vector<uint32_t> &indices) {
        size_t size = indices.size() * sizeof(uint32_t);
        mIndicesCount = indices.size();

        mStagingBuffer.updateBuffer(indices.data(), size);
        mIndexBuffer.copyFrom(commandPool, mStagingBuffer.getBuffer());

        return *this;
    }

} // vklite
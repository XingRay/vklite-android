//
// Created by leixing on 2025/3/13.
//

#include "HostVisibleIndexBuffer.h"

namespace vklite {
    HostVisibleIndexBuffer::HostVisibleIndexBuffer(const Device &device, vk::DeviceSize bufferSize)
            : mIndexBuffer(device, bufferSize, vk::BufferUsageFlagBits::eIndexBuffer) {

    }

    HostVisibleIndexBuffer::~HostVisibleIndexBuffer() = default;

    [[nodiscard]]
    const vk::Buffer &HostVisibleIndexBuffer::getBuffer() const {
        return mIndexBuffer.getBuffer();
    }

    [[nodiscard]]
    const vk::DeviceSize &HostVisibleIndexBuffer::getBufferSize() const {
        return mIndexBuffer.getBufferSize();
    }

    [[nodiscard]]
    const vk::DeviceMemory &HostVisibleIndexBuffer::getDeviceMemory() const {
        return mIndexBuffer.getDeviceMemory();
    }

    uint32_t HostVisibleIndexBuffer::getIndicesCount() const {
        return mIndicesCount;
    }

    void HostVisibleIndexBuffer::update(const std::vector<uint32_t>& indices) {
        size_t size = indices.size() * sizeof(uint32_t);
        mIndicesCount = indices.size();

        mIndexBuffer.updateBuffer(indices.data(), size);
    }

} // vklite
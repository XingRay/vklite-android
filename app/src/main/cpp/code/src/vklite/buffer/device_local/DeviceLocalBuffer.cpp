//
// Created by leixing on 2025/5/31.
//

#include "DeviceLocalBuffer.h"

namespace vklite {

    DeviceLocalBuffer::DeviceLocalBuffer(CombinedMemoryBuffer &&combinedMemoryBuffer)
            : mCombinedMemoryBuffer(std::move(combinedMemoryBuffer)) {}

    DeviceLocalBuffer::~DeviceLocalBuffer() = default;

    DeviceLocalBuffer::DeviceLocalBuffer(DeviceLocalBuffer &&other) noexcept
            : mCombinedMemoryBuffer(std::move(other.mCombinedMemoryBuffer)) {}

    DeviceLocalBuffer &DeviceLocalBuffer::operator=(DeviceLocalBuffer &&other) noexcept {
        if (this != &other) {
            mCombinedMemoryBuffer = std::move(other.mCombinedMemoryBuffer);
        }
        return *this;
    }

    const CombinedMemoryBuffer &DeviceLocalBuffer::getCombinedMemoryBuffer() const {
        return mCombinedMemoryBuffer;
    }

    const vk::Buffer &DeviceLocalBuffer::getBuffer() const {
        return mCombinedMemoryBuffer.getBuffer().getBuffer();
    }

    void DeviceLocalBuffer::recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) const {
        mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, srcBuffer, srcOffset, dstOffset, copyDataSize);
    }

    void DeviceLocalBuffer::recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer) const {
        mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, srcBuffer);
    }

    void DeviceLocalBuffer::copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset) const {
        mCombinedMemoryBuffer.getBuffer().copyFrom(commandPool, srcBuffer, srcOffset, copyDataSize, dstOffset);
    }

    void DeviceLocalBuffer::copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer) const {
        mCombinedMemoryBuffer.getBuffer().copyFrom(commandPool, srcBuffer);
    }

} // vklite
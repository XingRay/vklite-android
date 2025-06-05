//
// Created by leixing on 2025/5/31.
//

#include "CombinedMemoryBuffer.h"

namespace vklite {

    CombinedMemoryBuffer::CombinedMemoryBuffer(Buffer &&buffer, DeviceMemory &&deviceMemory)
            : mBuffer(std::move(buffer)), mDeviceMemory(std::move(deviceMemory)) {}

    CombinedMemoryBuffer::~CombinedMemoryBuffer() = default;

    CombinedMemoryBuffer::CombinedMemoryBuffer(CombinedMemoryBuffer &&other) noexcept
            : mBuffer(std::move(other.mBuffer)),
              mDeviceMemory(std::move(other.mDeviceMemory)) {}

    CombinedMemoryBuffer &CombinedMemoryBuffer::operator=(CombinedMemoryBuffer &&other) noexcept {
        mBuffer = std::move(other.mBuffer);
        mDeviceMemory = std::move(other.mDeviceMemory);
        return *this;
    }

    const Buffer &CombinedMemoryBuffer::getBuffer() const {
        return mBuffer;
    }

    const vk::Buffer &CombinedMemoryBuffer::getVkBuffer() const {
        return mBuffer.getVkBuffer();
    }

    const DeviceMemory &CombinedMemoryBuffer::getDeviceMemory() const {
        return mDeviceMemory;
    }

} // vklite
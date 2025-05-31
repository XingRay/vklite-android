//
// Created by leixing on 2025/1/16.
//

#include "HostVisibleBuffer.h"

#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    HostVisibleBuffer::HostVisibleBuffer(CombinedMemoryBuffer &&combinedMemoryBuffer)
            : mCombinedMemoryBuffer(std::move(combinedMemoryBuffer)) {}

    HostVisibleBuffer::~HostVisibleBuffer() = default;

    HostVisibleBuffer::HostVisibleBuffer(HostVisibleBuffer &&other) noexcept
            : mCombinedMemoryBuffer(std::move(other.mCombinedMemoryBuffer)) {}

    HostVisibleBuffer &HostVisibleBuffer::operator=(HostVisibleBuffer &&other) noexcept {
        if (this != &other) {
            mCombinedMemoryBuffer = std::move(other.mCombinedMemoryBuffer);
        }
        return *this;
    }


    const vk::Buffer &HostVisibleBuffer::getBuffer() const {
        return mCombinedMemoryBuffer.getBuffer().getBuffer();
    }

    const vk::DeviceSize &HostVisibleBuffer::getBufferSize() const {
        return mCombinedMemoryBuffer.getBuffer().getSize();
    }

    void HostVisibleBuffer::updateBuffer(const void *data, uint32_t size) {
        mCombinedMemoryBuffer.getDeviceMemory().updateBuffer(data, size);
    }

}
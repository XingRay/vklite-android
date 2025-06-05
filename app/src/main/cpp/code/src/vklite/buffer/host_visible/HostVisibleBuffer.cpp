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

    const CombinedMemoryBuffer &HostVisibleBuffer::getCombinedMemoryBuffer() const {
        return mCombinedMemoryBuffer;
    }

    const Buffer &HostVisibleBuffer::getBuffer() const {
        return mCombinedMemoryBuffer.getBuffer();
    }

    const vk::Buffer &HostVisibleBuffer::getVkBuffer() const {
        return mCombinedMemoryBuffer.getVkBuffer();
    }

    vk::DeviceSize HostVisibleBuffer::getBufferSize() const {
        return mCombinedMemoryBuffer.getBuffer().getSize();
    }

    void HostVisibleBuffer::updateBuffer(const void *data, uint32_t size, uint32_t offset, vk::MemoryMapFlags flags) const {
        mCombinedMemoryBuffer.getDeviceMemory().updateBuffer(data, size, offset, flags);
    }

    void HostVisibleBuffer::updateBuffer(const void *data, uint32_t size) const {
        updateBuffer(data, size, 0);
    }

}
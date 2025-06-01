//
// Created by leixing on 2025/3/6.
//

#include "StagingBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    StagingBuffer::StagingBuffer(CombinedMemoryBuffer &&combinedMemoryBuffer)
            : mCombinedMemoryBuffer(std::move(combinedMemoryBuffer)) {}

    StagingBuffer::~StagingBuffer() = default;

    StagingBuffer::StagingBuffer(StagingBuffer &&other) noexcept
            : mCombinedMemoryBuffer(std::move(other.mCombinedMemoryBuffer)) {}

    StagingBuffer &StagingBuffer::operator=(StagingBuffer &&other) noexcept {
        if (this != &other) {
            mCombinedMemoryBuffer = std::move(other.mCombinedMemoryBuffer);
        }
        return *this;
    }

    const vk::Buffer &StagingBuffer::getBuffer() const {
        return mCombinedMemoryBuffer.getBuffer().getBuffer();
    }

    vk::DeviceSize StagingBuffer::getSize() const {
        return mCombinedMemoryBuffer.getBuffer().getSize();
    }

    void StagingBuffer::updateBuffer(const void *data, uint32_t size, uint32_t offset, vk::MemoryMapFlags flags) {
        mCombinedMemoryBuffer.getDeviceMemory().updateBuffer(data, size, offset, flags);
    }

    void StagingBuffer::updateBuffer(const void *data, uint32_t size) {
        updateBuffer(data, size, 0);
    }

} // vklite
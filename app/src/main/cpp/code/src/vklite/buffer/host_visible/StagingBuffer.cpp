//
// Created by leixing on 2025/3/6.
//

#include "StagingBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    StagingBuffer::StagingBuffer(const PhysicalDevice &physicalDevice, const Device &device, vk::DeviceSize bufferSize)
            : mHostVisibleBuffer(physicalDevice, device, bufferSize, vk::BufferUsageFlagBits::eTransferSrc) {}

    StagingBuffer::~StagingBuffer() = default;

    const vk::Buffer &StagingBuffer::getBuffer() const {
        return mHostVisibleBuffer.getBuffer();
    }

    void StagingBuffer::updateBuffer(const void *data, uint32_t size) {
        mHostVisibleBuffer.updateBuffer(data, size);
    }

} // vklite
//
// Created by leixing on 2025/3/6.
//

#include "VulkanStagingBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    VulkanStagingBuffer::VulkanStagingBuffer(const Device &device, vk::DeviceSize bufferSize)
            : mVulkanHostVisibleBuffer(device, bufferSize, vk::BufferUsageFlagBits::eTransferSrc) {

    }

    VulkanStagingBuffer::~VulkanStagingBuffer() = default;

    const vk::Buffer &VulkanStagingBuffer::getBuffer() const {
        return mVulkanHostVisibleBuffer.getBuffer();
    }

    void VulkanStagingBuffer::updateBuffer(const void *data, uint32_t size) {
        mVulkanHostVisibleBuffer.updateBuffer(data, size);
    }

} // vklite
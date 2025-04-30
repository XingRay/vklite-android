//
// Created by leixing on 2025/3/13.
//

#include "VulkanDeviceLocalIndexBuffer.h"

namespace vklite {

    VulkanDeviceLocalIndexBuffer::VulkanDeviceLocalIndexBuffer(const Device &device, vk::DeviceSize bufferSize)
            : mIndexBuffer(device, bufferSize, vk::BufferUsageFlagBits::eIndexBuffer),
              mStagingBuffer(device, bufferSize) {

    }

    VulkanDeviceLocalIndexBuffer::~VulkanDeviceLocalIndexBuffer() = default;

    [[nodiscard]]
    const vk::Buffer &VulkanDeviceLocalIndexBuffer::getBuffer() const {
        return mIndexBuffer.getBuffer();
    }

    [[nodiscard]]
    const vk::DeviceMemory &VulkanDeviceLocalIndexBuffer::getDeviceMemory() const {
        return mIndexBuffer.getDeviceMemory();
    }

    [[nodiscard]]
    uint32_t VulkanDeviceLocalIndexBuffer::getIndicesCount() const {
        return mIndicesCount;
    }

    void VulkanDeviceLocalIndexBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<uint32_t> &indices) {
        size_t size = indices.size() * sizeof(uint32_t);
        mIndicesCount = indices.size();

        mStagingBuffer.updateBuffer(indices.data(), size);
        mIndexBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void VulkanDeviceLocalIndexBuffer::update(const VulkanCommandPool &vulkanCommandPool, const std::vector<uint32_t> &indices) {
        size_t size = indices.size() * sizeof(uint32_t);
        mIndicesCount = indices.size();

        mStagingBuffer.updateBuffer(indices.data(), size);
        mIndexBuffer.copyFrom(vulkanCommandPool, mStagingBuffer.getBuffer());
    }

} // vklite
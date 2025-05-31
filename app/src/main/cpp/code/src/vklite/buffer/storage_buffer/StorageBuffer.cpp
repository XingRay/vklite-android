//
// Created by leixing on 2024/12/31.
//

#include "StorageBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    StorageBuffer::StorageBuffer(const PhysicalDevice &physicalDevice,
                                 const Device &device,
                                 vk::DeviceSize bufferSize,
                                 vk::BufferUsageFlags additionalUsageFlags)
            : mDevice(device),
              mStagingBuffer(physicalDevice, device, bufferSize),
              mStorageBuffer(physicalDevice, device, bufferSize, vk::BufferUsageFlagBits::eStorageBuffer | additionalUsageFlags) {
    }

    StorageBuffer::~StorageBuffer() = default;

    const vk::Buffer &StorageBuffer::getBuffer() const {
        return mStorageBuffer.getBuffer();
    }

    const vk::DeviceMemory &StorageBuffer::getDeviceMemory() const {
        return mStorageBuffer.getDeviceMemory();
    }

    void StorageBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mStorageBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void StorageBuffer::update(const CommandPool &commandPool, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mStorageBuffer.copyFrom(commandPool, mStagingBuffer.getBuffer());
    }

    std::vector<vk::DescriptorBufferInfo> StorageBuffer::createDescriptorBufferInfos() {
        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mStorageBuffer.getBuffer())
                .setOffset(0)
                .setRange(mStorageBuffer.getBufferSize());

        std::vector<vk::DescriptorBufferInfo> descriptorBufferInfos = {descriptorBufferInfo};
        return descriptorBufferInfos;
    }

} // vklite
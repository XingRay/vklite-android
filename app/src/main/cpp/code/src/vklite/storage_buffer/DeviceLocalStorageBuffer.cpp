//
// Created by leixing on 2024/12/31.
//

#include "DeviceLocalStorageBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    DeviceLocalStorageBuffer::DeviceLocalStorageBuffer(const Device &device,
                                                       vk::DeviceSize bufferSize,
                                                       vk::BufferUsageFlags additionalUsageFlags)
            : mDevice(device),
              mStagingBuffer(device, bufferSize),
              mStorageBuffer(device, bufferSize, vk::BufferUsageFlagBits::eStorageBuffer | additionalUsageFlags) {
    }

    DeviceLocalStorageBuffer::~DeviceLocalStorageBuffer() = default;

    const vk::Buffer &DeviceLocalStorageBuffer::getBuffer() const {
        return mStorageBuffer.getBuffer();
    }

    const vk::DeviceMemory &DeviceLocalStorageBuffer::getDeviceMemory() const {
        return mStorageBuffer.getDeviceMemory();
    }

    void DeviceLocalStorageBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mStorageBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void DeviceLocalStorageBuffer::update(const CommandPool &commandPool, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mStorageBuffer.copyFrom(commandPool, mStagingBuffer.getBuffer());
    }

    std::vector<vk::DescriptorBufferInfo> DeviceLocalStorageBuffer::createDescriptorBufferInfos() {
        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mStorageBuffer.getBuffer())
                .setOffset(0)
                .setRange(mStorageBuffer.getBufferSize());

        std::vector<vk::DescriptorBufferInfo> descriptorBufferInfos = {descriptorBufferInfo};
        return descriptorBufferInfos;
    }

} // vklite
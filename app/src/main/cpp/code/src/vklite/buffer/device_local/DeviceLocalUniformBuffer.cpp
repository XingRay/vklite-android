//
// Created by leixing on 2025/3/13.
//

#include "DeviceLocalUniformBuffer.h"

namespace vklite {

    DeviceLocalUniformBuffer::DeviceLocalUniformBuffer(const Device &device, vk::DeviceSize bufferSize)
            : mUniformBuffer(device, bufferSize, vk::BufferUsageFlagBits::eUniformBuffer),
              mStagingBuffer(device, bufferSize) {

    }

    DeviceLocalUniformBuffer::~DeviceLocalUniformBuffer() = default;

    vk::DeviceSize DeviceLocalUniformBuffer::getSize() const {
        return mUniformBuffer.getBufferSize();
    }

    const vk::Buffer &DeviceLocalUniformBuffer::getBuffer() const {
        return mUniformBuffer.getBuffer();
    }

    const vk::DeviceMemory &DeviceLocalUniformBuffer::getDeviceMemory() const {
        return mUniformBuffer.getDeviceMemory();
    }

    void DeviceLocalUniformBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mUniformBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void DeviceLocalUniformBuffer::update(const CommandPool &commandPool, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mUniformBuffer.copyFrom(commandPool, mStagingBuffer.getBuffer());
    }

    std::vector<vk::DescriptorBufferInfo> DeviceLocalUniformBuffer::createDescriptorBufferInfos() {
        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mUniformBuffer.getBuffer())
                .setOffset(0)
                .setRange(mUniformBuffer.getBufferSize());

        std::vector<vk::DescriptorBufferInfo> descriptorBufferInfos = {descriptorBufferInfo};
        return descriptorBufferInfos;
    }

} // vklite
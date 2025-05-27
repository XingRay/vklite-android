//
// Created by leixing on 2025/3/13.
//

#include "UniformBuffer.h"

namespace vklite {

    UniformBuffer::UniformBuffer(const PhysicalDevice &physicalDevice, const Device &device, vk::DeviceSize bufferSize)
            : mUniformBuffer(physicalDevice, device, bufferSize, vk::BufferUsageFlagBits::eUniformBuffer),
              mStagingBuffer(physicalDevice, device, bufferSize) {

    }

    UniformBuffer::~UniformBuffer() = default;

    vk::DeviceSize UniformBuffer::getSize() const {
        return mUniformBuffer.getBufferSize();
    }

    const vk::Buffer &UniformBuffer::getBuffer() const {
        return mUniformBuffer.getBuffer();
    }

    const vk::DeviceMemory &UniformBuffer::getDeviceMemory() const {
        return mUniformBuffer.getDeviceMemory();
    }

    void UniformBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mUniformBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void UniformBuffer::update(const CommandPool &commandPool, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mUniformBuffer.copyFrom(commandPool, mStagingBuffer.getBuffer());
    }

    std::vector<vk::DescriptorBufferInfo> UniformBuffer::createDescriptorBufferInfos() {
        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mUniformBuffer.getBuffer())
                .setOffset(0)
                .setRange(mUniformBuffer.getBufferSize());

        std::vector<vk::DescriptorBufferInfo> descriptorBufferInfos = {descriptorBufferInfo};
        return descriptorBufferInfos;
    }

} // vklite
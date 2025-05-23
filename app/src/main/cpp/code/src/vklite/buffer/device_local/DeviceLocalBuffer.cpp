//
// Created by leixing on 2025/1/16.
//

#include "DeviceLocalBuffer.h"

#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    DeviceLocalBuffer::DeviceLocalBuffer(const Device &device, vk::DeviceSize bufferSize, vk::BufferUsageFlags bufferUsageFlagBits)
            : mDevice(device), mBufferSize(bufferSize) {

        vk::Device vkDevice = mDevice.getDevice();
        PhysicalDevice physicalDevice = mDevice.getPhysicalDevice();

        vk::BufferCreateInfo bufferCreateInfo{};
        bufferCreateInfo.setSize(bufferSize)
                .setUsage(vk::BufferUsageFlagBits::eTransferDst | bufferUsageFlagBits)
                .setSharingMode(vk::SharingMode::eExclusive);

        mBuffer = vkDevice.createBuffer(bufferCreateInfo);


        vk::MemoryRequirements memoryRequirements = vkDevice.getBufferMemoryRequirements(mBuffer);
        vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getPhysicalDevice().getMemoryProperties();

        uint32_t memoryType = physicalDevice.findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
        vk::MemoryAllocateInfo memoryAllocateInfo{};
        memoryAllocateInfo
                .setAllocationSize(memoryRequirements.size)
                .setMemoryTypeIndex(memoryType);

        vk::DeviceMemory bufferMemory = vkDevice.allocateMemory(memoryAllocateInfo);


        vkDevice.bindBufferMemory(mBuffer, bufferMemory, 0);
    }

    vklite::DeviceLocalBuffer::~DeviceLocalBuffer() {
        const vk::Device &vkDevice = mDevice.getDevice();

        vkDevice.unmapMemory(mDeviceMemory);
        vkDevice.destroy(mBuffer);
        vkDevice.freeMemory(mDeviceMemory);
    }

    const vk::Buffer &DeviceLocalBuffer::getBuffer() const {
        return mBuffer;
    }

    vk::DeviceSize DeviceLocalBuffer::getBufferSize() const {
        return mBufferSize;
    }

    const vk::DeviceMemory &DeviceLocalBuffer::getDeviceMemory() const {
        return mDeviceMemory;
    }

    void DeviceLocalBuffer::recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) {
        vk::BufferCopy bufferCopy;
        bufferCopy
                .setSrcOffset(srcOffset)
                .setDstOffset(dstOffset)
                .setSize(copyDataSize);
        commandBuffer.copyBuffer(srcBuffer, mBuffer, bufferCopy);
    }

    void DeviceLocalBuffer::recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer) {
        recordCommandCopyFrom(commandBuffer, srcBuffer, 0, 0, mBufferSize);
    }

    void DeviceLocalBuffer::copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyFrom(commandBuffer, srcBuffer, srcOffset, copyDataSize, dstOffset);
        });
    }

    void DeviceLocalBuffer::copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyFrom(commandBuffer, srcBuffer);
        });
    }
}
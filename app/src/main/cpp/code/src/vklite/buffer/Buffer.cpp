//
// Created by leixing on 2025/1/16.
//

#include "Buffer.h"

#include <utility>

#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

//    Buffer::Buffer(const PhysicalDevice &physicalDevice, const Device &device, vk::DeviceSize bufferSize, vk::BufferUsageFlags bufferUsageFlagBits)
//            : mDevice(device), mBufferSize(bufferSize) {
//
//        vk::Device vkDevice = mDevice.getDevice();
//
//        vk::BufferCreateInfo bufferCreateInfo{};
//        bufferCreateInfo.setSize(bufferSize)
//                .setUsage(vk::BufferUsageFlagBits::eTransferDst | bufferUsageFlagBits)
//                .setSharingMode(vk::SharingMode::eExclusive);
//
//        mBuffer = vkDevice.createBuffer(bufferCreateInfo);
//
//
//        vk::MemoryRequirements memoryRequirements = vkDevice.getBufferMemoryRequirements(mBuffer);
//        vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getPhysicalDevice().getMemoryProperties();
//
//        uint32_t memoryType = physicalDevice.findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
//        vk::MemoryAllocateInfo memoryAllocateInfo{};
//        memoryAllocateInfo
//                .setAllocationSize(memoryRequirements.size)
//                .setMemoryTypeIndex(memoryType);
//
//        vk::DeviceMemory bufferMemory = vkDevice.allocateMemory(memoryAllocateInfo);
//
//
//        vkDevice.bindBufferMemory(mBuffer, bufferMemory, 0);
//    }

    Buffer::Buffer(vk::Device device, vk::Buffer buffer, BufferMeta &&meta)
            : mDevice(device), mBuffer(buffer), mMeta(std::move(meta)) {}

    vklite::Buffer::~Buffer() {
        if (mDevice != nullptr && mBuffer != nullptr) {
            mDevice.destroy(mBuffer);
        }
    }

    Buffer::Buffer(Buffer &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mBuffer(std::exchange(other.mBuffer, nullptr)),
              mMeta(std::move(other.mMeta)) {}

    Buffer &Buffer::operator=(Buffer &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mBuffer = std::exchange(other.mBuffer, nullptr);
            mMeta = std::move(other.mMeta);
        }
        return *this;
    }

    const vk::Buffer &Buffer::getBuffer() const {
        return mBuffer;
    }

    vk::DeviceSize Buffer::getSize() const {
        return mMeta.getSize();
    }

    Buffer &Buffer::bindMemory(vk::DeviceMemory deviceMemory, vk::DeviceSize memoryOffset) {
        mDevice.bindBufferMemory(mBuffer, deviceMemory, memoryOffset);
        return *this;
    }

    void Buffer::recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) const {
        vk::BufferCopy bufferCopy;
        bufferCopy
                .setSrcOffset(srcOffset)
                .setDstOffset(dstOffset)
                .setSize(copyDataSize);
        commandBuffer.copyBuffer(srcBuffer, mBuffer, bufferCopy);
    }

    void Buffer::recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer) const {
        recordCommandCopyFrom(commandBuffer, srcBuffer, 0, 0, getSize());
    }

    void Buffer::copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset) const {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyFrom(commandBuffer, srcBuffer, srcOffset, copyDataSize, dstOffset);
        });
    }

    void Buffer::copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer) const {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyFrom(commandBuffer, srcBuffer);
        });
    }
}
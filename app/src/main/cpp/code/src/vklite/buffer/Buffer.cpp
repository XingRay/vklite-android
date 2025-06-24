//
// Created by leixing on 2025/1/16.
//

#include "Buffer.h"

#include <utility>

#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    Buffer::Buffer(vk::Device device, vk::Buffer buffer, BufferMeta &&meta)
            : mDevice(device), mVkBuffer(buffer), mMeta(std::move(meta)) {}

    vklite::Buffer::~Buffer() {
        if (mDevice != nullptr && mVkBuffer != nullptr) {
//            LOG_D("mDevice.destroy(mVkBuffer); mVkBuffer => %p", (void *) mVkBuffer);
            mDevice.destroy(mVkBuffer);
        }
    }

    Buffer::Buffer(Buffer &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mVkBuffer(std::exchange(other.mVkBuffer, nullptr)),
              mMeta(std::move(other.mMeta)) {}

    Buffer &Buffer::operator=(Buffer &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mVkBuffer = std::exchange(other.mVkBuffer, nullptr);
            mMeta = std::move(other.mMeta);
        }
        return *this;
    }

    const vk::Buffer &Buffer::getVkBuffer() const {
        return mVkBuffer;
    }

    vk::DeviceSize Buffer::getSize() const {
        return mMeta.getSize();
    }

    Buffer &Buffer::bindMemory(vk::DeviceMemory deviceMemory, vk::DeviceSize memoryOffset) {
        mDevice.bindBufferMemory(mVkBuffer, deviceMemory, memoryOffset);
        return *this;
    }

    void Buffer::recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) const {
        vk::BufferCopy bufferCopy;
        bufferCopy
                .setSrcOffset(srcOffset)
                .setDstOffset(dstOffset)
                .setSize(copyDataSize);
        commandBuffer.copyBuffer(srcBuffer, mVkBuffer, bufferCopy);
    }

    void Buffer::recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer) const {
        recordCommandCopyFrom(commandBuffer, srcBuffer, 0, 0, getSize());
    }

    void Buffer::copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset) const {
        commandPool.submit([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyFrom(commandBuffer, srcBuffer, srcOffset, copyDataSize, dstOffset);
        });
    }

    void Buffer::copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer) const {
        commandPool.submit([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyFrom(commandBuffer, srcBuffer);
        });
    }
}
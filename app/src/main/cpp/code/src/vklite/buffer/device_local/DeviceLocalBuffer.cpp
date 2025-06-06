//
// Created by leixing on 2025/5/31.
//

#include "DeviceLocalBuffer.h"

#include <utility>

#include "vklite/buffer/staging_buffer/StagingBufferBuilder.h"

namespace vklite {

    DeviceLocalBuffer::DeviceLocalBuffer(const vk::Device &device,
                                         CombinedMemoryBuffer &&combinedMemoryBuffer,
                                         std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties)
            : mDevice(device),
              mCombinedMemoryBuffer(std::move(combinedMemoryBuffer)),
              mPhysicalDeviceMemoryProperties(physicalDeviceMemoryProperties) {}

    DeviceLocalBuffer::~DeviceLocalBuffer() = default;

    DeviceLocalBuffer::DeviceLocalBuffer(DeviceLocalBuffer &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mCombinedMemoryBuffer(std::move(other.mCombinedMemoryBuffer)),
              mPhysicalDeviceMemoryProperties(other.mPhysicalDeviceMemoryProperties) {}

    DeviceLocalBuffer &DeviceLocalBuffer::operator=(DeviceLocalBuffer &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mCombinedMemoryBuffer = std::move(other.mCombinedMemoryBuffer);
            mPhysicalDeviceMemoryProperties = other.mPhysicalDeviceMemoryProperties;
        }
        return *this;
    }

    const CombinedMemoryBuffer &DeviceLocalBuffer::getCombinedMemoryBuffer() const {
        return mCombinedMemoryBuffer;
    }

    const Buffer &DeviceLocalBuffer::getBuffer() const {
        return mCombinedMemoryBuffer.getBuffer();
    }

    const vk::Buffer &DeviceLocalBuffer::getVkBuffer() const {
        return mCombinedMemoryBuffer.getVkBuffer();
    }

    DeviceLocalBuffer &DeviceLocalBuffer::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        return *this;
    }

    DeviceLocalBuffer &DeviceLocalBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer,
                                                       vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) {
        mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, srcBuffer, srcOffset, dstOffset, copyDataSize);
        return *this;
    }

    DeviceLocalBuffer &DeviceLocalBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer,
                                                       vk::DeviceSize copyDataSize) {
        recordUpdate(commandBuffer, srcBuffer, 0, 0, copyDataSize);
        return *this;
    }

    DeviceLocalBuffer &DeviceLocalBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer) {
        recordUpdate(commandBuffer, stagingBuffer.getVkBuffer(), 0, 0, stagingBuffer.getSize());
        return *this;
    }

    /**
     * 这个方法不能生效, 在函数内生成的 StagingBuffer 会在函数退出时清除, 此时 CommandBuffer 录制的指令还没有真正的执行,
     * 等到 CommandBuffer 提交执行时, StagingBuffer 已经被释放,会导致数据复制了空数据
     */
//    DeviceLocalBuffer &DeviceLocalBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size) {
//        if (!mPhysicalDeviceMemoryProperties.has_value()) {
//            throw std::runtime_error("mPhysicalDeviceMemoryProperties not set, must invoke DeviceLocalBuffer::physicalDeviceMemoryProperties()");
//        }
//        StagingBuffer stagingBuffer = StagingBufferBuilder()
//                .device(mDevice)
//                .size(size)
//                .physicalDeviceMemoryProperties(mPhysicalDeviceMemoryProperties.value())
//                .build();
//        stagingBuffer.updateBuffer(data, size);
//
//        mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, stagingBuffer.getBuffer());
//
//        return *this;
//    }


    DeviceLocalBuffer &DeviceLocalBuffer::update(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, srcBuffer, srcOffset, dstOffset, copyDataSize);
        });
        return *this;
    }

    DeviceLocalBuffer &DeviceLocalBuffer::update(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize copyDataSize) {
        update(commandPool, srcBuffer, 0, 0, copyDataSize);
        return *this;
    }

    DeviceLocalBuffer &DeviceLocalBuffer::update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer) {
        update(commandPool, stagingBuffer.getVkBuffer(), 0, 0, stagingBuffer.getSize());
        return *this;
    }

    DeviceLocalBuffer &DeviceLocalBuffer::update(const CommandPool &commandPool, const void *data, uint32_t size) {
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("mPhysicalDeviceMemoryProperties not set, must invoke DeviceLocalBuffer::configDeviceMemory()");
        }
        StagingBuffer stagingBuffer = StagingBufferBuilder()
                .device(mDevice)
                .size(size)
                .physicalDeviceMemoryProperties(mPhysicalDeviceMemoryProperties.value())
                .build();
        stagingBuffer.updateBuffer(data, size);

        mCombinedMemoryBuffer.getBuffer().copyFrom(commandPool, stagingBuffer.getVkBuffer());
        return *this;
    }

} // vklite
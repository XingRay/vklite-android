//
// Created by leixing on 2024/12/31.
//

#include "StorageBuffer.h"

#include <utility>

#include "vklite/buffer/staging_buffer/StagingBufferBuilder.h"

namespace vklite {

    StorageBuffer::StorageBuffer(const vk::Device &device,
                                 CombinedMemoryBuffer &&combinedMemoryBuffer,
                                 std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties)
            : mDevice(device),
              mCombinedMemoryBuffer(std::move(combinedMemoryBuffer)),
              mPhysicalDeviceMemoryProperties(physicalDeviceMemoryProperties) {}

    StorageBuffer::~StorageBuffer() = default;

    StorageBuffer::StorageBuffer(StorageBuffer &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mCombinedMemoryBuffer(std::move(other.mCombinedMemoryBuffer)),
              mPhysicalDeviceMemoryProperties(other.mPhysicalDeviceMemoryProperties) {}

    StorageBuffer &StorageBuffer::operator=(StorageBuffer &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mCombinedMemoryBuffer = std::move(other.mCombinedMemoryBuffer);
            mPhysicalDeviceMemoryProperties = other.mPhysicalDeviceMemoryProperties;
        }
        return *this;
    }

    const CombinedMemoryBuffer &StorageBuffer::getCombinedMemoryBuffer() const {
        return mCombinedMemoryBuffer;
    }

    const Buffer &StorageBuffer::getBuffer() const {
        return mCombinedMemoryBuffer.getBuffer();
    }

    const vk::Buffer &StorageBuffer::getVkBuffer() const {
        return mCombinedMemoryBuffer.getVkBuffer();
    }

    StorageBuffer &StorageBuffer::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        return *this;
    }

    StorageBuffer &StorageBuffer::physicalDeviceMemoryProperties(vk::PhysicalDevice physicalDevice) {
        mPhysicalDeviceMemoryProperties = physicalDevice.getMemoryProperties();
        return *this;
    }


    StorageBuffer &StorageBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) {
        mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, stagingBuffer, srcOffset, dstOffset, copyDataSize);
        return *this;
    }

    StorageBuffer &StorageBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize) {
        recordUpdate(commandBuffer, stagingBuffer, 0, 0, copyDataSize);
        return *this;
    }

    StorageBuffer &StorageBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer) {
        recordUpdate(commandBuffer, stagingBuffer.getVkBuffer(), 0, 0, stagingBuffer.getSize());
        return *this;
    }


    StorageBuffer &StorageBuffer::update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) {
        commandPool.submit([&](const vk::CommandBuffer &commandBuffer) {
            mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, stagingBuffer, srcOffset, dstOffset, copyDataSize);
        });
        return *this;
    }

    StorageBuffer &StorageBuffer::update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize) {
        update(commandPool, stagingBuffer, 0, 0, copyDataSize);
        return *this;
    }

    StorageBuffer &StorageBuffer::update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer) {
        update(commandPool, stagingBuffer.getVkBuffer(), 0, 0, stagingBuffer.getSize());
        return *this;
    }

    StorageBuffer &StorageBuffer::update(const CommandPool &commandPool, const void *data, uint32_t size) {
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("mPhysicalDeviceMemoryProperties not set, must invoke StorageBuffer::physicalDeviceMemoryProperties()");
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
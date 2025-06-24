//
// Created by leixing on 2025/3/13.
//

#include "VertexBuffer.h"

#include <utility>

#include "vklite/buffer/staging_buffer/StagingBufferBuilder.h"

namespace vklite {

    VertexBuffer::VertexBuffer(const vk::Device &device,
                               CombinedMemoryBuffer &&combinedMemoryBuffer,
                               std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties)
            : mDevice(device),
              mCombinedMemoryBuffer(std::move(combinedMemoryBuffer)),
              mPhysicalDeviceMemoryProperties(physicalDeviceMemoryProperties) {}

    VertexBuffer::~VertexBuffer() = default;

    VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mCombinedMemoryBuffer(std::move(other.mCombinedMemoryBuffer)),
              mPhysicalDeviceMemoryProperties(other.mPhysicalDeviceMemoryProperties) {}

    VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mCombinedMemoryBuffer = std::move(other.mCombinedMemoryBuffer);
            mPhysicalDeviceMemoryProperties = other.mPhysicalDeviceMemoryProperties;
        }
        return *this;
    }

    const CombinedMemoryBuffer &VertexBuffer::getCombinedMemoryBuffer() const {
        return mCombinedMemoryBuffer;
    }

    const Buffer &VertexBuffer::getBuffer() const {
        return mCombinedMemoryBuffer.getBuffer();
    }

    const vk::Buffer &VertexBuffer::getVkBuffer() const {
        return mCombinedMemoryBuffer.getVkBuffer();
    }

    VertexBuffer &VertexBuffer::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        return *this;
    }

    VertexBuffer &VertexBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset,
                                             vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) {
        mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, stagingBuffer, srcOffset, dstOffset, copyDataSize);
        return *this;
    }

    VertexBuffer &VertexBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize) {
        recordUpdate(commandBuffer, stagingBuffer, 0, 0, copyDataSize);
        return *this;
    }

    VertexBuffer &VertexBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer) {
        recordUpdate(commandBuffer, stagingBuffer.getVkBuffer(), 0, 0, stagingBuffer.getSize());
        return *this;
    }

    VertexBuffer &VertexBuffer::update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) {
        commandPool.submit([&](const vk::CommandBuffer &commandBuffer) {
            mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, stagingBuffer, srcOffset, dstOffset, copyDataSize);
        });
        return *this;
    }

    VertexBuffer &VertexBuffer::update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize) {
        update(commandPool, stagingBuffer, 0, 0, copyDataSize);
        return *this;
    }

    VertexBuffer &VertexBuffer::update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer) {
        update(commandPool, stagingBuffer.getVkBuffer(), 0, 0, stagingBuffer.getSize());
        return *this;
    }

    VertexBuffer &VertexBuffer::update(const CommandPool &commandPool, const void *data, uint32_t size) {
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("mPhysicalDeviceMemoryProperties not set, must invoke VertexBuffer::physicalDeviceMemoryProperties()");
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
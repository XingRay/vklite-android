//
// Created by leixing on 2025/3/13.
//

#include "IndexBuffer.h"
#include "vklite/buffer/staging_buffer/StagingBufferBuilder.h"
#include <utility>

namespace vklite {

    IndexBuffer::IndexBuffer(const vk::Device &device,
                             CombinedMemoryBuffer &&combinedMemoryBuffer,
                             std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties,
                             IndexBufferMeta &&meta)
            : mDevice(device),
              mCombinedMemoryBuffer(std::move(combinedMemoryBuffer)),
              mPhysicalDeviceMemoryProperties(physicalDeviceMemoryProperties),
              mMeta(std::move(meta)) {}

    IndexBuffer::~IndexBuffer() = default;

    IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mCombinedMemoryBuffer(std::move(other.mCombinedMemoryBuffer)),
              mPhysicalDeviceMemoryProperties(other.mPhysicalDeviceMemoryProperties),
              mMeta(std::move(other.mMeta)) {}

    IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mCombinedMemoryBuffer = std::move(other.mCombinedMemoryBuffer);
            mPhysicalDeviceMemoryProperties = other.mPhysicalDeviceMemoryProperties;
            mMeta = std::move(other.mMeta);
        }
        return *this;
    }

    const vk::Buffer &IndexBuffer::getBuffer() const {
        return mCombinedMemoryBuffer.getBuffer().getBuffer();
    }

    vk::IndexType IndexBuffer::getIndexType() const {
        return mMeta.getIndexType();
    }

    [[nodiscard]]
    uint32_t IndexBuffer::getIndicesCount() const {
        return mMeta.getIndicesCount();
    }

    IndexBuffer &IndexBuffer::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        return *this;
    }

    IndexBuffer &IndexBuffer::physicalDeviceMemoryProperties(vk::PhysicalDevice physicalDevice) {
        mPhysicalDeviceMemoryProperties = physicalDevice.getMemoryProperties();
        return *this;
    }


    IndexBuffer &IndexBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) {
        mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, stagingBuffer, srcOffset, dstOffset, copyDataSize);
        return *this;
    }

    IndexBuffer &IndexBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize) {
        recordUpdate(commandBuffer, stagingBuffer, 0, 0, copyDataSize);
        return *this;
    }

    IndexBuffer &IndexBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer) {
        recordUpdate(commandBuffer, stagingBuffer.getBuffer(), 0, 0, stagingBuffer.getSize());
        return *this;
    }

    IndexBuffer &IndexBuffer::recordUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<uint32_t> &indices) {
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("mPhysicalDeviceMemoryProperties not set, must invoke IndexBuffer::physicalDeviceMemoryProperties()");
        }
        vk::DeviceSize size = indices.size() * sizeof(uint32_t);
        StagingBuffer stagingBuffer = StagingBufferBuilder()
                .device(mDevice)
                .size(size)
                .physicalDeviceMemoryProperties(mPhysicalDeviceMemoryProperties.value())
                .build();
        stagingBuffer.updateBuffer(indices.data(), size);

        mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, stagingBuffer.getBuffer());

        return *this;
    }


    IndexBuffer &IndexBuffer::update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, stagingBuffer, srcOffset, dstOffset, copyDataSize);
        });
        return *this;
    }

    IndexBuffer &IndexBuffer::update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize) {
        update(commandPool, stagingBuffer, 0, 0, copyDataSize);
        return *this;
    }

    IndexBuffer &IndexBuffer::update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer) {
        update(commandPool, stagingBuffer.getBuffer(), 0, 0, stagingBuffer.getSize());
        return *this;
    }

    IndexBuffer &IndexBuffer::update(const CommandPool &commandPool, const std::vector<uint32_t> &indices) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordUpdate(commandBuffer, indices);
        });
        return *this;
    }

} // vklite
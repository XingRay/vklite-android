//
// Created by leixing on 2025/3/13.
//

#include "IndexBuffer.h"
#include "vklite/buffer/staging_buffer/StagingBufferBuilder.h"
#include <utility>

namespace vklite {

    IndexBuffer::IndexBuffer(const vk::Device &device, CombinedMemoryBuffer &&combinedMemoryBuffer, IndexBufferMeta &&meta)
            : mDevice(device), mCombinedMemoryBuffer(std::move(combinedMemoryBuffer)), mMeta(std::move(meta)) {}

    IndexBuffer::~IndexBuffer() = default;

    IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mCombinedMemoryBuffer(std::move(other.mCombinedMemoryBuffer)),
              mMeta(std::move(other.mMeta)) {}

    IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mCombinedMemoryBuffer = std::move(other.mCombinedMemoryBuffer);
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

    IndexBuffer &IndexBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<uint32_t> &indices) {
        StagingBuffer stagingBuffer = StagingBufferBuilder()
                .device(mDevice)
                .physicalDeviceMemoryProperties(mPhysicalDeviceMemoryProperties)
                .build();
        stagingBuffer.updateBuffer(indices.data(), indices.size() * sizeof(uint32_t));

        mCombinedMemoryBuffer.getBuffer().recordCommandCopyFrom(commandBuffer, stagingBuffer.getBuffer());

        return *this;
    }

    IndexBuffer &IndexBuffer::update(const CommandPool &commandPool, const std::vector<uint32_t> &indices) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer& commandBuffer){
            recordCommandUpdate(commandBuffer, indices);
        });
        return *this;
    }

} // vklite
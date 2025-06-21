//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "vklite/buffer/device_local/DeviceLocalBuffer.h"
#include "vklite/buffer/staging_buffer/StagingBuffer.h"
#include "vklite/command_pool/CommandPool.h"
#include "vklite/buffer/index_buffer/IndexBufferMeta.h"

namespace vklite {

    class IndexBuffer {
    private:
        vk::Device mDevice;
        CombinedMemoryBuffer mCombinedMemoryBuffer;
        IndexBufferMeta mMeta;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

    public:
        explicit IndexBuffer(const vk::Device &device,
                             CombinedMemoryBuffer &&combinedMemoryBuffer,
                             std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties,
                             IndexBufferMeta &&meta);

        ~IndexBuffer();

        IndexBuffer(const IndexBuffer &other) = delete;

        IndexBuffer &operator=(const IndexBuffer &other) = delete;

        IndexBuffer(IndexBuffer &&other) noexcept;

        IndexBuffer &operator=(IndexBuffer &&other) noexcept;

        [[nodiscard]]
        const CombinedMemoryBuffer &getCombinedMemoryBuffer() const;

        [[nodiscard]]
        const Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::Buffer &getVkBuffer() const;

        [[nodiscard]]
        vk::IndexType getIndexType() const;

        IndexBuffer &physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        IndexBuffer &physicalDeviceMemoryProperties(vk::PhysicalDevice physicalDevice);


        // recordUpdate
        IndexBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        IndexBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize);

        IndexBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer);

        IndexBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<uint32_t> &indices);


        // update
        IndexBuffer &update(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        IndexBuffer &update(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize copyDataSize);

        IndexBuffer &update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer);

        IndexBuffer &update(const CommandPool &commandPool, const std::vector<uint32_t> &indices);
    };

} // vklite

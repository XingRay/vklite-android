//
// Created by leixing on 2025/3/13.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBuffer.h"
#include "vklite/buffer/staging_buffer/StagingBuffer.h"
#include "vklite/command_pool/CommandPool.h"


namespace vklite {

    class VertexBuffer {
    private:
        vk::Device mDevice;
        CombinedMemoryBuffer mCombinedMemoryBuffer;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

    public:
        explicit VertexBuffer(const vk::Device &device,
                              CombinedMemoryBuffer &&combinedMemoryBuffer,
                              std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties);

        ~VertexBuffer();

        VertexBuffer(const VertexBuffer &other) = delete;

        VertexBuffer &operator=(const VertexBuffer &other) = delete;

        VertexBuffer(VertexBuffer &&other) noexcept;

        VertexBuffer &operator=(VertexBuffer &&other) noexcept;

        [[nodiscard]]
        const CombinedMemoryBuffer &getCombinedMemoryBuffer() const;

        [[nodiscard]]
        const Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::Buffer &getVkBuffer() const;

        VertexBuffer &physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        // recordUpdate
        VertexBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        VertexBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize);

        VertexBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer);


        // update
        VertexBuffer &update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        VertexBuffer &update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize);

        VertexBuffer &update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer);

        VertexBuffer &update(const CommandPool &commandPool, const void *data, uint32_t size);

    };

} // vklite

//
// Created by leixing on 2024/12/31.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBuffer.h"
#include "vklite/buffer/staging_buffer/StagingBuffer.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class StorageBuffer {
    private:
        vk::Device mDevice;
        CombinedMemoryBuffer mCombinedMemoryBuffer;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

    public:
        explicit StorageBuffer(const vk::Device &device,
                               CombinedMemoryBuffer &&combinedMemoryBuffer,
                               std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties);

        ~StorageBuffer();

        StorageBuffer(const StorageBuffer &other) = delete;

        StorageBuffer &operator=(const StorageBuffer &other) = delete;

        StorageBuffer(StorageBuffer &&other) noexcept;

        StorageBuffer &operator=(StorageBuffer &&other) noexcept;

        [[nodiscard]]
        const CombinedMemoryBuffer &getCombinedMemoryBuffer() const;

        [[nodiscard]]
        const Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::Buffer &getVkBuffer() const;

        StorageBuffer &physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        StorageBuffer &physicalDeviceMemoryProperties(vk::PhysicalDevice physicalDevice);

        // recordUpdate
        StorageBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        StorageBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize);

        StorageBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer);


        // update
        StorageBuffer &update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        StorageBuffer &update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize);

        StorageBuffer &update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer);

        StorageBuffer &update(const CommandPool &commandPool, const void *data, uint32_t size);
    };

} // vklite

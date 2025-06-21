//
// Created by leixing on 2025/3/13.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBuffer.h"
#include "vklite/buffer/staging_buffer/StagingBuffer.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class UniformBuffer {
    private:
        vk::Device mDevice;
        CombinedMemoryBuffer mCombinedMemoryBuffer;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

    public:
        explicit UniformBuffer(const vk::Device &device,
                               CombinedMemoryBuffer &&combinedMemoryBuffer,
                               std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties);

        ~UniformBuffer();

        UniformBuffer(const UniformBuffer &other) = delete;

        UniformBuffer &operator=(const UniformBuffer &other) = delete;

        UniformBuffer(UniformBuffer &&other) noexcept;

        UniformBuffer &operator=(UniformBuffer &&other) noexcept;

        [[nodiscard]]
        const CombinedMemoryBuffer &getCombinedMemoryBuffer() const;

        [[nodiscard]]
        const Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::Buffer &getVkBuffer() const;

        UniformBuffer &configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        UniformBuffer &configDeviceMemory(vk::PhysicalDevice physicalDevice);

        // recordUpdate
        UniformBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        UniformBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize);

        UniformBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer);


        // update
        UniformBuffer &update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        UniformBuffer &update(const CommandPool &commandPool, vk::Buffer stagingBuffer, vk::DeviceSize copyDataSize);

        UniformBuffer &update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer);

        UniformBuffer &update(const CommandPool &commandPool, const void *data, uint32_t size);
    };

} // vklite

//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBuffer.h"
#include "vklite/buffer/staging_buffer/StagingBuffer.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class DeviceLocalBuffer {
    private:
        vk::Device mDevice;
        CombinedMemoryBuffer mCombinedMemoryBuffer;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

    public:
        explicit DeviceLocalBuffer(const vk::Device &device,
                                   CombinedMemoryBuffer &&combinedMemoryBuffer,
                                   std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties);

        ~DeviceLocalBuffer();

        DeviceLocalBuffer(const DeviceLocalBuffer &other) = delete;

        DeviceLocalBuffer &operator=(const DeviceLocalBuffer &other) = delete;

        DeviceLocalBuffer(DeviceLocalBuffer &&other) noexcept;

        DeviceLocalBuffer &operator=(DeviceLocalBuffer &&other) noexcept;

        [[nodiscard]]
        const CombinedMemoryBuffer &getCombinedMemoryBuffer() const;

        [[nodiscard]]
        const Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::Buffer &getVkBuffer() const;

        DeviceLocalBuffer &physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        // recordUpdate
        DeviceLocalBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        DeviceLocalBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize copyDataSize);

        DeviceLocalBuffer &recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer);


        // update
        DeviceLocalBuffer &update(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        DeviceLocalBuffer &update(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize copyDataSize);

        DeviceLocalBuffer &update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer);

        DeviceLocalBuffer &update(const CommandPool &commandPool, const void *data, uint32_t size);

    };

} // vklite

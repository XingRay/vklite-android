//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBuffer.h"

namespace vklite {

    class DeviceLocalBuffer {
    private:
        CombinedMemoryBuffer mCombinedMemoryBuffer;

    public:
        DeviceLocalBuffer(CombinedMemoryBuffer &&combinedMemoryBuffer);

        ~DeviceLocalBuffer();

        DeviceLocalBuffer(const DeviceLocalBuffer &other) = delete;

        DeviceLocalBuffer &operator=(const DeviceLocalBuffer &other) = delete;

        DeviceLocalBuffer(DeviceLocalBuffer &&other) noexcept;

        DeviceLocalBuffer &operator=(DeviceLocalBuffer &&other) noexcept;

        [[nodiscard]]
        const CombinedMemoryBuffer &getCombinedMemoryBuffer() const;

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        void recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) const;

        void recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer) const;

        void copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset) const;

        void copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer) const;
    };

} // vklite

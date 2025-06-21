//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

#include "vklite/buffer/BufferMeta.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    /**
     * buffer on gpu, and invisible on cpu.
     */
    class Buffer {
    private:
        vk::Device mDevice;
        vk::Buffer mVkBuffer;
        BufferMeta mMeta;

    public:
        explicit Buffer(vk::Device device, vk::Buffer buffer, BufferMeta &&meta);

        ~Buffer();

        Buffer(const Buffer &other) = delete;

        Buffer &operator=(const Buffer &other) = delete;

        Buffer(Buffer &&other) noexcept;

        Buffer &operator=(Buffer &&other) noexcept;

        [[nodiscard]]
        const vk::Buffer &getVkBuffer() const;

        [[nodiscard]]
        vk::DeviceSize getSize() const;

        Buffer &bindMemory(vk::DeviceMemory deviceMemory, vk::DeviceSize memoryOffset = 0);

        void recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) const;

        void recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer) const;

        void copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset) const;

        void copyFrom(const CommandPool &commandPool, vk::Buffer srcBuffer) const;
    };

} // vklite

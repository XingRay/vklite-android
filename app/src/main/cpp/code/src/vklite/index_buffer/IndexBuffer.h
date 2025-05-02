//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "vklite/buffer/device_local/VulkanDeviceLocalBuffer.h"
#include "vklite/buffer/host_visible/VulkanStagingBuffer.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class IndexBuffer {
    private:
        VulkanDeviceLocalBuffer mIndexBuffer;
        uint32_t mIndicesCount;

        VulkanStagingBuffer mStagingBuffer;

    public:
        IndexBuffer(const Device &device, vk::DeviceSize bufferSize);

        ~IndexBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        uint32_t getIndicesCount() const;

        IndexBuffer& recordCommandUpdate(const vk::CommandBuffer& commandBuffer, const std::vector<uint32_t>& indices);

        IndexBuffer& update(const CommandPool& vulkanCommandPool, const std::vector<uint32_t>& indices);
    };

} // vklite

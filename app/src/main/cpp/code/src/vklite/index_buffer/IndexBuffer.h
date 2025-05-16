//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "vklite/buffer/device_local/DeviceLocalBuffer.h"
#include "vklite/buffer/host_visible/StagingBuffer.h"
#include "vklite/command_buffer/CommandPool.h"

namespace vklite {

    class IndexBuffer {
    private:
        DeviceLocalBuffer mIndexBuffer;
        uint32_t mIndicesCount;

        StagingBuffer mStagingBuffer;

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

        IndexBuffer& update(const CommandPool& commandPool, const std::vector<uint32_t>& indices);
    };

} // vklite

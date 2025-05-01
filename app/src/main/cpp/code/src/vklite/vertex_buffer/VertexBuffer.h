//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "vklite/buffer/device_local/VulkanDeviceLocalBuffer.h"
#include "vklite/buffer/host_visible/VulkanStagingBuffer.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class VertexBuffer {
    private:
        VulkanDeviceLocalBuffer mVertexBuffer;
        VulkanStagingBuffer mStagingBuffer;

    public:
        VertexBuffer(const Device &device, vk::DeviceSize bufferSize);

        ~VertexBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size);

        template<class T>
        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<T> &data) {
            recordCommandUpdate(commandBuffer, data.data(), data.size() * sizeof(T));
        }

        void update(const CommandPool &vulkanCommandPool, const void *data, uint32_t size);

        template<class T>
        void update(const CommandPool &vulkanCommandPool, const std::vector<T> &data) {
            update(vulkanCommandPool, data.data(), data.size() * sizeof(T));
        }

    };

} // vklite

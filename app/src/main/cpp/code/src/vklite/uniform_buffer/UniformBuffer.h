//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "vklite/buffer/device_local/DeviceLocalBuffer.h"
#include "vklite/buffer/host_visible/StagingBuffer.h"
#include "vklite/command_buffer/CommandPool.h"
#include "vklite/buffer/BufferInterface.h"

namespace vklite {

    class UniformBuffer : public BufferInterface {
    private:
        DeviceLocalBuffer mUniformBuffer;
        StagingBuffer mStagingBuffer;

    public:
        UniformBuffer(const Device &device, vk::DeviceSize bufferSize);

        ~UniformBuffer() override;

        [[nodiscard]]
        vk::DeviceSize getSize() const override;

        [[nodiscard]]
        const vk::Buffer &getBuffer() const override;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size);

        template<class T>
        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<T> &data) {
            recordCommandUpdate(commandBuffer, data.data(), data.size() * sizeof(T));
        }

        void update(const CommandPool &commandPool, const void *data, uint32_t size) override;

        template<class T>
        void update(const CommandPool &commandPool, const std::vector<T> &data) {
            update(commandPool, data.data(), data.size() * sizeof(T));
        }

        std::vector<vk::DescriptorBufferInfo> createDescriptorBufferInfos();
    };

} // vklite

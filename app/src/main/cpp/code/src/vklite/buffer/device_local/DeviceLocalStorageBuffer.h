//
// Created by leixing on 2024/12/31.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/command_buffer/CommandPool.h"


#include "DeviceLocalBuffer.h"
#include "vklite/buffer/host_visible/StagingBuffer.h"

namespace vklite {

    class DeviceLocalStorageBuffer {
    private:
        const Device &mDevice;

        StagingBuffer mStagingBuffer;
        DeviceLocalBuffer mStorageBuffer;

    public:
        DeviceLocalStorageBuffer(const Device &device, vk::DeviceSize bufferSize, vk::BufferUsageFlags additionalUsageFlags = vk::BufferUsageFlags{});

        ~DeviceLocalStorageBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size);

        template<class T>
        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const std::vector<T> &data) {
            recordCommandUpdate(commandBuffer, data.data(), data.size() * sizeof(T));
        }

        void update(const CommandPool &commandPool, const void *data, uint32_t size);

        template<class T>
        void update(const CommandPool &commandPool, const std::vector<T> &data) {
            update(commandPool, data.data(), data.size() * sizeof(T));
        }

        [[nodiscard]]
        std::vector<vk::DescriptorBufferInfo> createDescriptorBufferInfos();
    };

} // vklite

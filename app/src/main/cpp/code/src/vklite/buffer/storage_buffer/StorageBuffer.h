//
// Created by leixing on 2024/12/31.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include "vklite/device/Device.h"
#include "vklite/command_pool/CommandPool.h"


#include "vklite/buffer/device_local/DeviceLocalBuffer.h"
#include "vklite/buffer/staging_buffer/StagingBuffer.h"

namespace vklite {

    class StorageBuffer {
    private:
        const Device &mDevice;

        StagingBuffer mStagingBuffer;
        DeviceLocalBuffer mStorageBuffer;

    public:
        StorageBuffer(const PhysicalDevice &physicalDevice, const Device &device, vk::DeviceSize bufferSize, vk::BufferUsageFlags additionalUsageFlags = vk::BufferUsageFlags{});

        ~StorageBuffer();

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

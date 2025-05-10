//
// Created by leixing on 2025/3/31.
//

#pragma once

#include <memory>
#include <cstdint>
#include <vector>
#include <functional>

#include "vklite/pipeline/resource/BufferInfo.h"
#include "vklite/buffer/BufferBuilderInterface.h"
#include "vklite/device/Device.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class DescriptorBufferInfoConfigure {
    private:
        // set bufferInfo
        std::unique_ptr<BufferInfo> mDescriptorBufferInfo;

        // create Buffer
        std::unique_ptr<BufferBuilderInterface> mBufferBuilder;
        vk::DeviceSize mBufferCapacity;

        // create bufferInfo
        uint32_t mBufferOffset;
        uint32_t mBufferRange;

        // update buffer
        std::vector<uint8_t> mBufferData;

    public:

        DescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo, const void *data, uint32_t size);

        DescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo, std::vector<uint8_t> &&data);

        explicit DescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo);

        DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                      uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, const void *data, uint32_t size);

        DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                      uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, std::vector<uint8_t> &&data);

        DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                      uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange);

        DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                      const void *data, uint32_t size);

        DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                      std::vector<uint8_t> &&data);

        DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity);

        template<class T>
        DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity, const T &data)
                :DescriptorBufferInfoConfigure(vulkanBufferBuilder, vulkanBufferCapacity, &data, sizeof(T)) {}

        ~DescriptorBufferInfoConfigure();

        [[nodiscard]]
        std::unique_ptr<BufferInfo> provideDescriptorBufferInfo(const Device &device, const CommandPool &commandPool);

    private:
        DescriptorBufferInfoConfigure &copyData(const void *data, uint32_t size);
    };

} // vklite

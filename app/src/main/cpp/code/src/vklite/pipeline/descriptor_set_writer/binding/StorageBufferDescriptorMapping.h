//
// Created by leixing on 2025/6/7.
//

#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/buffer/Buffer.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorMapping.h"

namespace vklite {

    class StorageBufferDescriptorMapping {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorIndex;
        std::vector<vk::DescriptorBufferInfo> mDescriptorBufferInfos;

    public:
        explicit StorageBufferDescriptorMapping();

        ~StorageBufferDescriptorMapping();

        StorageBufferDescriptorMapping(const StorageBufferDescriptorMapping &other) = delete;

        StorageBufferDescriptorMapping &operator=(const StorageBufferDescriptorMapping &other) = delete;

        StorageBufferDescriptorMapping(StorageBufferDescriptorMapping &&other) noexcept;

        StorageBufferDescriptorMapping &operator=(StorageBufferDescriptorMapping &&other) noexcept;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getDescriptorIndex() const;

        [[nodiscard]]
        std::vector<vk::DescriptorBufferInfo> &getDescriptorBufferInfos();

        StorageBufferDescriptorMapping &binding(uint32_t binding);

        StorageBufferDescriptorMapping &descriptorIndex(uint32_t descriptorIndex);

        // add buffer info
        StorageBufferDescriptorMapping &addBufferInfo(vk::DescriptorBufferInfo bufferInfo);

        StorageBufferDescriptorMapping &addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range);

        StorageBufferDescriptorMapping &addBufferInfo(vk::Buffer buffer, vk::DeviceSize range);

        StorageBufferDescriptorMapping &addBufferInfo(const Buffer &buffer);
    };

} // vklite

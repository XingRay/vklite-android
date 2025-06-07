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

    class StorageDescriptorMapping {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorIndex;
        std::vector<vk::DescriptorBufferInfo> mDescriptorBufferInfos;

    public:
        StorageDescriptorMapping();

        ~StorageDescriptorMapping();

        StorageDescriptorMapping(const StorageDescriptorMapping &other) = delete;

        StorageDescriptorMapping &operator=(const StorageDescriptorMapping &other) = delete;

        StorageDescriptorMapping(StorageDescriptorMapping &&other) noexcept;

        StorageDescriptorMapping &operator=(StorageDescriptorMapping &&other) noexcept;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getDescriptorIndex() const;

        [[nodiscard]]
        std::vector<vk::DescriptorBufferInfo> &getDescriptorBufferInfos();

        StorageDescriptorMapping &binding(uint32_t binding);

        StorageDescriptorMapping &descriptorIndex(uint32_t descriptorIndex);

        // add buffer info
        StorageDescriptorMapping &addBufferInfo(vk::DescriptorBufferInfo bufferInfo);

        StorageDescriptorMapping &addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range);

        StorageDescriptorMapping &addBufferInfo(const Buffer &buffer);
    };

} // vklite

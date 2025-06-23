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

    class UniformBufferDescriptorMapping {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorIndex;
        std::vector<vk::DescriptorBufferInfo> mDescriptorBufferInfos;

    public:
        explicit UniformBufferDescriptorMapping();

        ~UniformBufferDescriptorMapping();

        UniformBufferDescriptorMapping(const UniformBufferDescriptorMapping &other) = delete;

        UniformBufferDescriptorMapping &operator=(const UniformBufferDescriptorMapping &other) = delete;

        UniformBufferDescriptorMapping(UniformBufferDescriptorMapping &&other) noexcept;

        UniformBufferDescriptorMapping &operator=(UniformBufferDescriptorMapping &&other) noexcept;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getDescriptorIndex() const;

        [[nodiscard]]
        std::vector<vk::DescriptorBufferInfo> &getDescriptorBufferInfos();

        UniformBufferDescriptorMapping &binding(uint32_t binding);

        UniformBufferDescriptorMapping &descriptorIndex(uint32_t descriptorIndex);

        // add buffer info
        UniformBufferDescriptorMapping &addBufferInfo(vk::DescriptorBufferInfo bufferInfo);

        UniformBufferDescriptorMapping &addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range);

        UniformBufferDescriptorMapping &addBufferInfo(const Buffer &buffer);
    };

} // vklite

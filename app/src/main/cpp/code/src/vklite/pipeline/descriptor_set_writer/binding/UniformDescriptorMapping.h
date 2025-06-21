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

    class UniformDescriptorMapping {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorIndex;
        std::vector<vk::DescriptorBufferInfo> mDescriptorBufferInfos;

    public:
        explicit UniformDescriptorMapping();

        ~UniformDescriptorMapping();

        UniformDescriptorMapping(const UniformDescriptorMapping &other) = delete;

        UniformDescriptorMapping &operator=(const UniformDescriptorMapping &other) = delete;

        UniformDescriptorMapping(UniformDescriptorMapping &&other) noexcept;

        UniformDescriptorMapping &operator=(UniformDescriptorMapping &&other) noexcept;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getDescriptorIndex() const;

        [[nodiscard]]
        std::vector<vk::DescriptorBufferInfo> &getDescriptorBufferInfos();

        UniformDescriptorMapping &binding(uint32_t binding);

        UniformDescriptorMapping &descriptorIndex(uint32_t descriptorIndex);

        // add buffer info
        UniformDescriptorMapping &addBufferInfo(vk::DescriptorBufferInfo bufferInfo);

        UniformDescriptorMapping &addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range);

        UniformDescriptorMapping &addBufferInfo(const Buffer &buffer);
    };

} // vklite

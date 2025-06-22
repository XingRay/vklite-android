//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor/DescriptorBindingConfigure.h"

namespace vklite {

    class StorageBufferConfigure {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorCount;
        vk::ShaderStageFlags mShaderStageFlags;

    public:
        explicit StorageBufferConfigure(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags);

        StorageBufferConfigure();

        ~StorageBufferConfigure();

        StorageBufferConfigure &binding(uint32_t binding);

        StorageBufferConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        StorageBufferConfigure &descriptorCount(uint32_t descriptorCount);

        [[nodiscard]]
        DescriptorBindingConfigure createDescriptorBindingConfigure() const;
    };

} // vklite

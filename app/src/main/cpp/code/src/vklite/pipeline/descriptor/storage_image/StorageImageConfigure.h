//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor/DescriptorBindingConfigure.h"

namespace vklite {

    class StorageImageConfigure {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorCount;
        vk::ShaderStageFlags mShaderStageFlags;

    public:
        explicit StorageImageConfigure(uint32_t binding,
                                       uint32_t descriptorCount,
                                       vk::ShaderStageFlags shaderStageFlags);

        StorageImageConfigure();

        ~StorageImageConfigure();

        StorageImageConfigure &binding(uint32_t binding);

        StorageImageConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        StorageImageConfigure &descriptorCount(uint32_t descriptorCount);

        [[nodiscard]]
        DescriptorBindingConfigure createDescriptorBindingConfigure() const;
    };

} // vklite

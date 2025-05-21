//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"

#include "vklite/pipeline/descriptor/DescriptorBindingConfigure.h"

namespace vklite {

    class StorageConfigure {
    private:
        uint32_t mBinding;
        vk::ShaderStageFlags mShaderStageFlags;
        uint32_t mDescriptorCount;

    public:
        StorageConfigure();

        ~StorageConfigure();

        StorageConfigure &binding(uint32_t binding);

        StorageConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        StorageConfigure &descriptorCount(uint32_t descriptorCount);

        [[nodiscard]]
        DescriptorBindingConfigure createDescriptorBindingConfigure() const;
    };

} // vklite

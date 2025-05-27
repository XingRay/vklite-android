//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor/DescriptorBindingConfigure.h"

namespace vklite {

    class UniformConfigure {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorCount;
        vk::ShaderStageFlags mShaderStageFlags;

    public:
        UniformConfigure(uint32_t binding,
                         uint32_t descriptorCount,
                         vk::ShaderStageFlags shaderStageFlags);

        UniformConfigure();

        ~UniformConfigure();

        UniformConfigure &binding(uint32_t binding);

        UniformConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        UniformConfigure &descriptorCount(uint32_t descriptorCount);

        [[nodiscard]]
        DescriptorBindingConfigure createDescriptorBindingConfigure() const;
    };

} // vklite

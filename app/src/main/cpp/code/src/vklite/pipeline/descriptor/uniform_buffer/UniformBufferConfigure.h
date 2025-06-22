//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor/DescriptorBindingConfigure.h"

namespace vklite {

    class UniformBufferConfigure {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorCount;
        vk::ShaderStageFlags mShaderStageFlags;

    public:
        explicit UniformBufferConfigure(uint32_t binding,
                                        uint32_t descriptorCount,
                                        vk::ShaderStageFlags shaderStageFlags);

        UniformBufferConfigure();

        ~UniformBufferConfigure();

        UniformBufferConfigure &binding(uint32_t binding);

        UniformBufferConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        UniformBufferConfigure &descriptorCount(uint32_t descriptorCount);

        [[nodiscard]]
        DescriptorBindingConfigure createDescriptorBindingConfigure() const;
    };

} // vklite

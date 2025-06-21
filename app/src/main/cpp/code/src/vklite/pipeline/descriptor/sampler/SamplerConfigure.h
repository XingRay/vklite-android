//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor/DescriptorBindingConfigure.h"

namespace vklite {
    
    class SamplerConfigure {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorCount;
        vk::ShaderStageFlags mShaderStageFlags;

    public:
        explicit SamplerConfigure(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags);

        SamplerConfigure();

        ~SamplerConfigure();

        SamplerConfigure &binding(uint32_t binding);

        SamplerConfigure &descriptorCount(uint32_t descriptorCount);

        SamplerConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        [[nodiscard]]
        DescriptorBindingConfigure createDescriptorBindingConfigure() const;

    };

} // vklite

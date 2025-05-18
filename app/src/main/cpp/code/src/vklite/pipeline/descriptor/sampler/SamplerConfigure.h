//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"

#include "vklite/pipeline/descriptor/DescriptorBindingConfigure.h"

namespace vklite {


    class SamplerConfigure {
    private:
        uint32_t mBinding;
        vk::ShaderStageFlags mShaderStageFlags;
        uint32_t mDescriptorCount;

    public:
        SamplerConfigure();

        ~SamplerConfigure();

        SamplerConfigure &binding(uint32_t binding);

        SamplerConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        SamplerConfigure &descriptorCount(uint32_t descriptorCount);

        [[nodiscard]]
        DescriptorBindingConfigure createDescriptorBindingConfigure() const;

    };

} // vklite

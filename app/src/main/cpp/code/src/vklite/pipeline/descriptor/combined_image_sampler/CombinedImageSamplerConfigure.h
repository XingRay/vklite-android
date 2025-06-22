//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor/DescriptorBindingConfigure.h"

namespace vklite {
    
    class CombinedImageSamplerConfigure {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorCount;
        vk::ShaderStageFlags mShaderStageFlags;

    public:
        explicit CombinedImageSamplerConfigure(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags);

        CombinedImageSamplerConfigure();

        ~CombinedImageSamplerConfigure();

        CombinedImageSamplerConfigure &binding(uint32_t binding);

        CombinedImageSamplerConfigure &descriptorCount(uint32_t descriptorCount);

        CombinedImageSamplerConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        [[nodiscard]]
        DescriptorBindingConfigure createDescriptorBindingConfigure() const;

    };

} // vklite

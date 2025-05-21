//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <functional>

#include "vulkan/vulkan.hpp"

#include "vklite/pipeline/descriptor/DescriptorBindingConfigure.h"
#include "vklite/sampler/SamplerInterface.h"

namespace vklite {

    class ImmutableSamplerConfigure {

    private:
        uint32_t mBinding;
        vk::ShaderStageFlags mShaderStageFlags;
        std::vector<vk::Sampler> mSamplers;

    public:
        ImmutableSamplerConfigure();

        ~ImmutableSamplerConfigure();

        ImmutableSamplerConfigure &binding(uint32_t binding);

        ImmutableSamplerConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        ImmutableSamplerConfigure &addSampler(vk::Sampler sampler);

        ImmutableSamplerConfigure &addSampler(const SamplerInterface &sampler);

        [[nodiscard]]
        DescriptorBindingConfigure createDescriptorBindingConfigure();
    };

} // vklite

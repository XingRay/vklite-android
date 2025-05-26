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
        std::vector<vk::Sampler> mSamplers;
        vk::ShaderStageFlags mShaderStageFlags;

    public:
        ImmutableSamplerConfigure(uint32_t binding, std::vector<vk::Sampler> &&samplers, vk::ShaderStageFlags shaderStageFlags);

        ImmutableSamplerConfigure();

        ~ImmutableSamplerConfigure();

        ImmutableSamplerConfigure &binding(uint32_t binding);

        ImmutableSamplerConfigure &addSampler(vk::Sampler sampler);

        ImmutableSamplerConfigure &addSampler(const SamplerInterface &sampler);

        ImmutableSamplerConfigure &addSamplers(std::vector<vk::Sampler> &&samplers);

        ImmutableSamplerConfigure &addSamplers(const std::vector<SamplerInterface> &samplers);

        ImmutableSamplerConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

        [[nodiscard]]
        DescriptorBindingConfigure createDescriptorBindingConfigure();
    };

} // vklite

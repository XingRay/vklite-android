//
// Created by leixing on 2025/3/24.
//

#include "ImmutableSamplerConfigure.h"

namespace vklite {

    ImmutableSamplerConfigure::ImmutableSamplerConfigure(uint32_t binding, std::vector<vk::Sampler> &&samplers, vk::ShaderStageFlags shaderStageFlags)
            : mBinding(binding), mSamplers(std::move(samplers)), mShaderStageFlags(shaderStageFlags) {}

    ImmutableSamplerConfigure::ImmutableSamplerConfigure()
            : ImmutableSamplerConfigure(0, {}, vk::ShaderStageFlagBits::eFragment) {}

    ImmutableSamplerConfigure::~ImmutableSamplerConfigure() = default;

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::addSampler(vk::Sampler sampler) {
        mSamplers.push_back(sampler);
        return *this;
    }

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::addSampler(const Sampler &sampler) {
        addSampler(sampler.getVkSampler());
        return *this;
    }

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::addSamplers(std::vector<vk::Sampler> &&samplers) {
        mSamplers.insert(mSamplers.end(),
                         std::make_move_iterator(samplers.begin()),
                         std::make_move_iterator(samplers.end()));
        return *this;
    }

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::addSamplers(const std::vector<Sampler> &samplers) {
        for (const auto &sampler: samplers) {
            addSampler(sampler);
        }
        return *this;
    }

    DescriptorBindingConfigure ImmutableSamplerConfigure::createDescriptorBindingConfigure() {
        return DescriptorBindingConfigure{
                mBinding,
                vk::DescriptorType::eCombinedImageSampler,
                std::move(mSamplers),
                mShaderStageFlags
        };
    }

} // vklite
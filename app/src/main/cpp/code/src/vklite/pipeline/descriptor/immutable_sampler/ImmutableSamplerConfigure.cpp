//
// Created by leixing on 2025/3/24.
//

#include "ImmutableSamplerConfigure.h"

namespace vklite {

    ImmutableSamplerConfigure::ImmutableSamplerConfigure()
            : mBinding(0), mShaderStageFlags(vk::ShaderStageFlagBits::eFragment) {}

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

    ImmutableSamplerConfigure &ImmutableSamplerConfigure::addSampler(const SamplerInterface &sampler) {
        addSampler(sampler.getSampler());
        return *this;
    }

    DescriptorBindingConfigure ImmutableSamplerConfigure::createDescriptorBindingConfigure() {
//        return DescriptorBindingConfigure(mBinding, vk::DescriptorType::eUniformBuffer, mShaderStageFlags, mDescriptorCount);
        return {mBinding, vk::DescriptorType::eCombinedImageSampler, mShaderStageFlags, std::move(mSamplers)};
    }

} // vklite
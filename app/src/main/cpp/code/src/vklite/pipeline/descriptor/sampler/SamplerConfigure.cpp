//
// Created by leixing on 2025/3/24.
//

#include "SamplerConfigure.h"

#include "vklite/image/ImageBuilder.h"
#include "vklite/sampler/SamplerBuilder.h"

namespace vklite {

    SamplerConfigure::SamplerConfigure()
            : mBinding(0), mDescriptorCount(1), mShaderStageFlags(vk::ShaderStageFlagBits::eFragment) {}

    SamplerConfigure::~SamplerConfigure() = default;

    SamplerConfigure &SamplerConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    SamplerConfigure &SamplerConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    SamplerConfigure &SamplerConfigure::descriptorCount(uint32_t descriptorCount) {
        mDescriptorCount = descriptorCount;
        return *this;
    }

    DescriptorBindingConfigure SamplerConfigure::createDescriptorBindingConfigure() const {
//        return DescriptorBindingConfigure(mBinding, vk::DescriptorType::eUniformBuffer, mShaderStageFlags, mDescriptorCount);
        return {mBinding, vk::DescriptorType::eCombinedImageSampler, mShaderStageFlags, mDescriptorCount};
    }

} // vklite
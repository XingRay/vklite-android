//
// Created by leixing on 2025/3/24.
//

#include "SamplerConfigure.h"

#include "vklite/image/ImageBuilder.h"
#include "vklite/sampler/SamplerBuilder.h"

namespace vklite {

    SamplerConfigure::SamplerConfigure(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags)
            : mBinding(binding), mDescriptorCount(descriptorCount), mShaderStageFlags(shaderStageFlags) {}

    SamplerConfigure::SamplerConfigure()
            : SamplerConfigure(0, 1, vk::ShaderStageFlagBits::eFragment) {}

    SamplerConfigure::~SamplerConfigure() = default;

    SamplerConfigure &SamplerConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    SamplerConfigure &SamplerConfigure::descriptorCount(uint32_t descriptorCount) {
        mDescriptorCount = descriptorCount;
        return *this;
    }

    SamplerConfigure &SamplerConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    DescriptorBindingConfigure SamplerConfigure::createDescriptorBindingConfigure() const {
        return DescriptorBindingConfigure{mBinding, vk::DescriptorType::eCombinedImageSampler, mDescriptorCount, mShaderStageFlags};
    }

} // vklite
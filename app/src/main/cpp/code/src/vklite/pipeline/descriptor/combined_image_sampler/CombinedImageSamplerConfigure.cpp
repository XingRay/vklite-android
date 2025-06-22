//
// Created by leixing on 2025/3/24.
//

#include "CombinedImageSamplerConfigure.h"

#include "vklite/image/ImageBuilder.h"
#include "vklite/sampler/SamplerBuilder.h"

namespace vklite {

    CombinedImageSamplerConfigure::CombinedImageSamplerConfigure(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags)
            : mBinding(binding), mDescriptorCount(descriptorCount), mShaderStageFlags(shaderStageFlags) {}

    CombinedImageSamplerConfigure::CombinedImageSamplerConfigure()
            : CombinedImageSamplerConfigure(0, 1, vk::ShaderStageFlagBits::eFragment) {}

    CombinedImageSamplerConfigure::~CombinedImageSamplerConfigure() = default;

    CombinedImageSamplerConfigure &CombinedImageSamplerConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    CombinedImageSamplerConfigure &CombinedImageSamplerConfigure::descriptorCount(uint32_t descriptorCount) {
        mDescriptorCount = descriptorCount;
        return *this;
    }

    CombinedImageSamplerConfigure &CombinedImageSamplerConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    DescriptorBindingConfigure CombinedImageSamplerConfigure::createDescriptorBindingConfigure() const {
        return DescriptorBindingConfigure{mBinding, vk::DescriptorType::eCombinedImageSampler, mDescriptorCount, mShaderStageFlags};
    }

} // vklite
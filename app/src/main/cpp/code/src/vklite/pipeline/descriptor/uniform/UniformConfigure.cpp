//
// Created by leixing on 2025/3/24.
//

#include "UniformConfigure.h"

namespace vklite {

    UniformConfigure::UniformConfigure(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags)
            : mBinding(binding), mDescriptorCount(descriptorCount), mShaderStageFlags(shaderStageFlags) {}

    UniformConfigure::UniformConfigure()
            : UniformConfigure(0, 1, vk::ShaderStageFlagBits::eVertex) {}

    UniformConfigure::~UniformConfigure() = default;

    UniformConfigure &UniformConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    UniformConfigure &UniformConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    UniformConfigure &UniformConfigure::descriptorCount(uint32_t descriptorCount) {
        mDescriptorCount = descriptorCount;
        return *this;
    }

    DescriptorBindingConfigure UniformConfigure::createDescriptorBindingConfigure() const {
        return DescriptorBindingConfigure{mBinding, vk::DescriptorType::eUniformBuffer, mDescriptorCount, mShaderStageFlags};
    }

} // vklite
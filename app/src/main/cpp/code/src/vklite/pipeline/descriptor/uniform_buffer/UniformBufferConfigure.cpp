//
// Created by leixing on 2025/3/24.
//

#include "UniformBufferConfigure.h"

namespace vklite {

    UniformBufferConfigure::UniformBufferConfigure(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags)
            : mBinding(binding), mDescriptorCount(descriptorCount), mShaderStageFlags(shaderStageFlags) {}

    UniformBufferConfigure::UniformBufferConfigure()
            : UniformBufferConfigure(0, 1, vk::ShaderStageFlagBits::eVertex) {}

    UniformBufferConfigure::~UniformBufferConfigure() = default;

    UniformBufferConfigure &UniformBufferConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    UniformBufferConfigure &UniformBufferConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    UniformBufferConfigure &UniformBufferConfigure::descriptorCount(uint32_t descriptorCount) {
        mDescriptorCount = descriptorCount;
        return *this;
    }

    DescriptorBindingConfigure UniformBufferConfigure::createDescriptorBindingConfigure() const {
        return DescriptorBindingConfigure{mBinding, vk::DescriptorType::eUniformBuffer, mDescriptorCount, mShaderStageFlags};
    }

} // vklite
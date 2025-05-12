//
// Created by leixing on 2025/3/24.
//

#include "UniformConfigure.h"
#include "vklite/pipeline/descriptor/old/DescriptorSlotConfigure.h"

namespace vklite {

    UniformConfigure::UniformConfigure()
            : mBinding(0), mDescriptorCount(1), mShaderStageFlags(vk::ShaderStageFlagBits::eVertex) {}

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
//        return DescriptorBindingConfigure(mBinding, vk::DescriptorType::eUniformBuffer, mShaderStageFlags, mDescriptorCount);
        return {mBinding, vk::DescriptorType::eUniformBuffer, mShaderStageFlags, mDescriptorCount, {}};
    }

} // vklite
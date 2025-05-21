//
// Created by leixing on 2025/3/24.
//

#include "StorageConfigure.h"

namespace vklite {

    StorageConfigure::StorageConfigure()
            : mBinding(0), mDescriptorCount(1), mShaderStageFlags(vk::ShaderStageFlagBits::eVertex) {}

    StorageConfigure::~StorageConfigure() = default;

    StorageConfigure &StorageConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    StorageConfigure &StorageConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    StorageConfigure &StorageConfigure::descriptorCount(uint32_t descriptorCount) {
        mDescriptorCount = descriptorCount;
        return *this;
    }

    DescriptorBindingConfigure StorageConfigure::createDescriptorBindingConfigure() const {
//        return DescriptorBindingConfigure(mBinding, vk::DescriptorType::eUniformBuffer, mShaderStageFlags, mDescriptorCount);
        return {mBinding, vk::DescriptorType::eUniformBuffer, mShaderStageFlags, mDescriptorCount};
    }

} // vklite
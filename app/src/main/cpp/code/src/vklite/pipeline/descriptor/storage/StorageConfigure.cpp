//
// Created by leixing on 2025/3/24.
//

#include "StorageConfigure.h"

namespace vklite {

    StorageConfigure::StorageConfigure(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags)
            : mBinding(binding), mDescriptorCount(descriptorCount), mShaderStageFlags(shaderStageFlags) {}

    StorageConfigure::StorageConfigure()
            : StorageConfigure(0, 1, vk::ShaderStageFlagBits::eVertex) {}

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
        return {mBinding, vk::DescriptorType::eStorageBuffer, mDescriptorCount, mShaderStageFlags};
    }

} // vklite
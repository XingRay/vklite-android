//
// Created by leixing on 2025/3/24.
//

#include "StorageBufferConfigure.h"

namespace vklite {

    StorageBufferConfigure::StorageBufferConfigure(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags)
            : mBinding(binding), mDescriptorCount(descriptorCount), mShaderStageFlags(shaderStageFlags) {}

    StorageBufferConfigure::StorageBufferConfigure()
            : StorageBufferConfigure(0, 1, vk::ShaderStageFlagBits::eVertex) {}

    StorageBufferConfigure::~StorageBufferConfigure() = default;

    StorageBufferConfigure &StorageBufferConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    StorageBufferConfigure &StorageBufferConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    StorageBufferConfigure &StorageBufferConfigure::descriptorCount(uint32_t descriptorCount) {
        mDescriptorCount = descriptorCount;
        return *this;
    }

    DescriptorBindingConfigure StorageBufferConfigure::createDescriptorBindingConfigure() const {
        return DescriptorBindingConfigure{mBinding, vk::DescriptorType::eStorageBuffer, mDescriptorCount, mShaderStageFlags};
    }

} // vklite
//
// Created by leixing on 2025/3/24.
//

#include "StorageImageConfigure.h"

namespace vklite {

    StorageImageConfigure::StorageImageConfigure(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags)
            : mBinding(binding), mDescriptorCount(descriptorCount), mShaderStageFlags(shaderStageFlags) {}

    StorageImageConfigure::StorageImageConfigure()
            : StorageImageConfigure(0, 1, vk::ShaderStageFlagBits::eCompute) {}

    StorageImageConfigure::~StorageImageConfigure() = default;

    StorageImageConfigure &StorageImageConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    StorageImageConfigure &StorageImageConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    StorageImageConfigure &StorageImageConfigure::descriptorCount(uint32_t descriptorCount) {
        mDescriptorCount = descriptorCount;
        return *this;
    }

    DescriptorBindingConfigure StorageImageConfigure::createDescriptorBindingConfigure() const {
        return DescriptorBindingConfigure{mBinding, vk::DescriptorType::eStorageImage, mDescriptorCount, mShaderStageFlags};
    }

} // vklite
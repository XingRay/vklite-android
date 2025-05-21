//
// Created by leixing on 2025/5/11.
//

#include "DescriptorBindingConfigure.h"

namespace vklite {

    DescriptorBindingConfigure::DescriptorBindingConfigure()
            : DescriptorBindingConfigure(0, vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex, 1) {}

    DescriptorBindingConfigure::DescriptorBindingConfigure(uint32_t binding,
                                                           vk::DescriptorType descriptorType,
                                                           vk::ShaderStageFlags shaderStageFlags,
                                                           uint32_t descriptorCount)
            : mBinding(binding),
              mDescriptorType(descriptorType),
              mShaderStageFlags(shaderStageFlags),
              mDescriptorCount(descriptorCount) {}

    DescriptorBindingConfigure::DescriptorBindingConfigure(uint32_t binding,
                                                           vk::DescriptorType descriptorType,
                                                           vk::ShaderStageFlags shaderStageFlags,
                                                           std::vector<vk::Sampler> &&immutableSamplers)
            : mBinding(binding),
              mDescriptorType(descriptorType),
              mShaderStageFlags(shaderStageFlags),
              mDescriptorCount(immutableSamplers.size()),
              mImmutableSamplers(std::move(immutableSamplers)) {}

    DescriptorBindingConfigure::~DescriptorBindingConfigure() = default;

    uint32_t DescriptorBindingConfigure::getBinding() const {
        return mBinding;
    }

    vk::DescriptorType DescriptorBindingConfigure::getDescriptorType() const {
        return mDescriptorType;
    }

    vk::ShaderStageFlags DescriptorBindingConfigure::getShaderStageFlags() const {
        return mShaderStageFlags;
    }

    uint32_t DescriptorBindingConfigure::getDescriptorCount() const {
        return mDescriptorCount;
    }

    const std::vector<vk::Sampler> &DescriptorBindingConfigure::getImmutableSamplers() const {
        return mImmutableSamplers;
    }

    vk::DescriptorSetLayoutBinding DescriptorBindingConfigure::createDescriptorSetLayoutBinding() const {
        vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding;
        if (mImmutableSamplers.empty()) {
            descriptorSetLayoutBinding
                    .setBinding(mBinding)
                    .setDescriptorType(mDescriptorType)
                    .setDescriptorCount(mDescriptorCount)
                    .setStageFlags(mShaderStageFlags);
//                    .setImmutableSamplers(mImmutableSamplers);
        } else {
            descriptorSetLayoutBinding
                    .setBinding(mBinding)
                    .setDescriptorType(mDescriptorType)
//                    .setDescriptorCount(mDescriptorCount)
                    .setStageFlags(mShaderStageFlags)
                            // ** 注意 ** setImmutableSamplers 会同时设置 descriptorCount 为 mImmutableSamplers.size()
                    .setImmutableSamplers(mImmutableSamplers);
        }
        return descriptorSetLayoutBinding;
    }

} // vklite
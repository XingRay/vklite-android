//
// Created by leixing on 2025/5/11.
//

#include "DescriptorBindingConfigure.h"

namespace vklite {

    DescriptorBindingConfigure::DescriptorBindingConfigure(uint32_t binding,
                                                           vk::DescriptorType descriptorType,
                                                           uint32_t descriptorCount,
                                                           vk::ShaderStageFlags shaderStageFlags)
            : mBinding(binding),
              mDescriptorType(descriptorType),
              mDescriptorCount(descriptorCount),
              mImmutableSamplers{},
              mShaderStageFlags(shaderStageFlags) {}

    DescriptorBindingConfigure::DescriptorBindingConfigure(uint32_t binding,
                                                           vk::DescriptorType descriptorType,
                                                           std::vector<vk::Sampler> &&immutableSamplers,
                                                           vk::ShaderStageFlags shaderStageFlags)
            : mBinding(binding),
              mDescriptorType(descriptorType),
              mDescriptorCount(immutableSamplers.size()),
              mImmutableSamplers(std::move(immutableSamplers)),
              mShaderStageFlags(shaderStageFlags) {}

    DescriptorBindingConfigure::DescriptorBindingConfigure()
            : DescriptorBindingConfigure(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex) {}

    DescriptorBindingConfigure::~DescriptorBindingConfigure() = default;

    DescriptorBindingConfigure::DescriptorBindingConfigure(const DescriptorBindingConfigure &other)
            : mBinding(other.mBinding),
              mDescriptorType(other.mDescriptorType),
              mShaderStageFlags(other.mShaderStageFlags),
              mDescriptorCount(other.mDescriptorCount),
              mImmutableSamplers(other.mImmutableSamplers) {}

    DescriptorBindingConfigure &DescriptorBindingConfigure::operator=(const DescriptorBindingConfigure &other) {
        if (this != &other) {
            mBinding = other.mBinding;
            mDescriptorType = other.mDescriptorType;
            mShaderStageFlags = other.mShaderStageFlags;
            mDescriptorCount = other.mDescriptorCount;
            mImmutableSamplers = other.mImmutableSamplers;
        }
        return *this;
    }

    DescriptorBindingConfigure::DescriptorBindingConfigure(DescriptorBindingConfigure &&other) noexcept
            : mBinding(other.mBinding),
              mDescriptorType(other.mDescriptorType),
              mShaderStageFlags(other.mShaderStageFlags),
              mDescriptorCount(other.mDescriptorCount),
              mImmutableSamplers(std::move(other.mImmutableSamplers)) {}

    DescriptorBindingConfigure &DescriptorBindingConfigure::operator=(DescriptorBindingConfigure &&other) noexcept {
        if (this != &other) {
            mBinding = other.mBinding;
            mDescriptorType = other.mDescriptorType;
            mShaderStageFlags = other.mShaderStageFlags;
            mDescriptorCount = other.mDescriptorCount;
            mImmutableSamplers = std::move(other.mImmutableSamplers);
        }
        return *this;
    }

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
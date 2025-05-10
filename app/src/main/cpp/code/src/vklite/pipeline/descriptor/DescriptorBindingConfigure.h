//
// Created by leixing on 2025/5/11.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class DescriptorBindingConfigure {
    private:
        uint32_t mBinding;
        vk::DescriptorType mDescriptorType;
        vk::ShaderStageFlags mShaderStageFlags;
        uint32_t mDescriptorCount;
        std::vector<vk::Sampler> mImmutableSamplers;

    public:

        DescriptorBindingConfigure();

        DescriptorBindingConfigure(uint32_t binding,
                                   vk::DescriptorType descriptorType,
                                   vk::ShaderStageFlags shaderStageFlags,
                                   uint32_t descriptorCount,
                                   std::vector<vk::Sampler> &&immutableSamplers);

        ~DescriptorBindingConfigure();

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        vk::DescriptorType getDescriptorType() const;

        [[nodiscard]]
        vk::ShaderStageFlags getShaderStageFlags() const;

        [[nodiscard]]
        uint32_t getDescriptorCount() const;

        [[nodiscard]]
        const std::vector<vk::Sampler> &getImmutableSamplers() const;

        [[nodiscard]]
        vk::DescriptorSetLayoutBinding createDescriptorSetLayoutBinding() const;
    };

} // vklite

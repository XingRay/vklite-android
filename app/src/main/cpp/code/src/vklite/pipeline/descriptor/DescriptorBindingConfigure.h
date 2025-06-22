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
        uint32_t mDescriptorCount;
        std::vector<vk::Sampler> mImmutableSamplers;
        vk::ShaderStageFlags mShaderStageFlags;

    public:
        explicit DescriptorBindingConfigure(uint32_t binding,
                                            vk::DescriptorType descriptorType,
                                            uint32_t descriptorCount,
                                            vk::ShaderStageFlags shaderStageFlags);

        explicit DescriptorBindingConfigure(uint32_t binding,
                                            vk::DescriptorType descriptorType,
                                            std::vector<vk::Sampler> &&immutableSamplers,
                                            vk::ShaderStageFlags shaderStageFlags);

        DescriptorBindingConfigure();

        ~DescriptorBindingConfigure();

        DescriptorBindingConfigure(const DescriptorBindingConfigure &other);

        DescriptorBindingConfigure &operator=(const DescriptorBindingConfigure &other);

        DescriptorBindingConfigure(DescriptorBindingConfigure &&other) noexcept;

        DescriptorBindingConfigure &operator=(DescriptorBindingConfigure &&other) noexcept;

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

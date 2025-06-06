//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"
#include "vklite/pipeline/descriptor/uniform/UniformConfigure.h"
#include "vklite/pipeline/descriptor/sampler/SamplerConfigure.h"
#include "vklite/pipeline/descriptor/immutable_sampler/ImmutableSamplerConfigure.h"
#include "vklite/pipeline/descriptor/storage/StorageConfigure.h"

namespace vklite {

    class DescriptorSetConfigure {
    private:
        uint32_t mSet;

        // binding -> BindingConfigure
        std::unordered_map<uint32_t, DescriptorBindingConfigure> mDescriptorBindingConfigures;

    public:

        DescriptorSetConfigure();

        ~DescriptorSetConfigure();

        DescriptorSetConfigure(const DescriptorSetConfigure &other);

        DescriptorSetConfigure &operator=(const DescriptorSetConfigure &other);

        DescriptorSetConfigure(DescriptorSetConfigure &&other) noexcept;

        DescriptorSetConfigure &operator=(DescriptorSetConfigure &&other) noexcept;

        [[nodiscard]]
        uint32_t getSet() const;

        [[nodiscard]]
        const std::unordered_map<uint32_t, DescriptorBindingConfigure> &getDescriptorBindingConfigures() const;

        DescriptorSetConfigure &set(uint32_t set);

        // descriptorBinding
        DescriptorSetConfigure &addDescriptorBinding(DescriptorBindingConfigure &&bindingConfigure);

        DescriptorSetConfigure &addDescriptorBinding(uint32_t binding,
                                                     vk::DescriptorType descriptorType,
                                                     uint32_t descriptorCount,
                                                     vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addDescriptorBinding(uint32_t binding,
                                                     vk::DescriptorType descriptorType,
                                                     std::vector<vk::Sampler> &&immutableSamplers,
                                                     vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addDescriptorBinding(const std::function<void(DescriptorBindingConfigure &)> &configure);


        // uniform
        DescriptorSetConfigure &addUniform(const UniformConfigure &configure);

        DescriptorSetConfigure &addUniform(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addUniform(const std::function<void(UniformConfigure &)> &configure);


        // sampler
        DescriptorSetConfigure &addSampler(const SamplerConfigure &configure);

        DescriptorSetConfigure &addSampler(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addSampler(uint32_t binding, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addSampler(const std::function<void(SamplerConfigure &)> &configure);


        // immutable sampler
        DescriptorSetConfigure &addImmutableSampler(ImmutableSamplerConfigure &configure);

        DescriptorSetConfigure &addImmutableSampler(uint32_t binding, std::vector<vk::Sampler> &&samplers, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addImmutableSampler(const std::function<void(ImmutableSamplerConfigure &)> &configure);


        //storage buffer
        DescriptorSetConfigure &addStorage(const StorageConfigure &configure);

        DescriptorSetConfigure &addStorage(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addStorage(const std::function<void(StorageConfigure &)> &configure);

        //todo: addXxx()

        [[nodiscard]]
        std::vector<vk::DescriptorSetLayoutBinding> createDescriptorSetLayoutBindings() const;

    };

} // vklite

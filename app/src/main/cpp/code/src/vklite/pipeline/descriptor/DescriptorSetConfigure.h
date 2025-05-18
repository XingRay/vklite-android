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

namespace vklite {

    class DescriptorSetConfigure {
    private:
        uint32_t mSet;

        // binding -> BindingConfigure
        std::unordered_map<uint32_t, DescriptorBindingConfigure> mDescriptorBindingConfigures;

    public:

        DescriptorSetConfigure();

        ~DescriptorSetConfigure();

        [[nodiscard]]
        uint32_t getSet() const;

        [[nodiscard]]
        const std::unordered_map<uint32_t, DescriptorBindingConfigure> &getDescriptorBindingConfigures() const;

        DescriptorSetConfigure &set(uint32_t set);

        // descriptorBinding
        DescriptorSetConfigure &addDescriptorBinding(const std::function<void(DescriptorBindingConfigure &)> &configure);

        DescriptorSetConfigure &addDescriptorBinding(DescriptorBindingConfigure &&bindingConfigure);


        // uniform
        DescriptorSetConfigure &addUniform(const UniformConfigure &configure);

        DescriptorSetConfigure &addUniform(const std::function<void(UniformConfigure &)> &configure);


        // sampler
        DescriptorSetConfigure &addSampler(const SamplerConfigure &configure);

        DescriptorSetConfigure &addSampler(const std::function<void(SamplerConfigure &)> &configure);


        //DescriptorSetConfigure &addImmutableSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

//        DescriptorSetConfigure &addImmutableSampler(const std::function<void(VulkanImmutableSamplerConfigure &)> &configure);

//        DescriptorSetConfigure &addStorage(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

//        std::unique_ptr<DescriptorBindingSet> createDescriptorBindingSet(const Device &device, const CommandPool &commandPool);

        std::vector<vk::DescriptorSetLayoutBinding> createDescriptorSetLayoutBindings() const;

    private:
//        std::vector<vk::DescriptorSetLayoutBinding> createDescriptorSetLayoutBindings();

//        vk::DescriptorSetLayout createDescriptorSetLayout(const VulkanDevice &device);
    };

} // vklite

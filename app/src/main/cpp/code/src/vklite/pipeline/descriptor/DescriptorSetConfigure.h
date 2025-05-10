//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"
#include "vklite/pipeline/descriptor/old/DescriptorSlotConfigure.h"
#include "vklite/pipeline/descriptor/old/DescriptorBindingSet.h"
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

//        const std::unordered_map<uint32_t, std::unique_ptr<DescriptorBindingConfigure>> &getDescriptorBindingConfigures();

        DescriptorSetConfigure &set(uint32_t set);

        // addDescriptorBinding
        DescriptorSetConfigure &addDescriptorBinding(const std::function<void(DescriptorBindingConfigure &)> &configure);

        DescriptorSetConfigure &addDescriptorBinding(DescriptorBindingConfigure &&bindingConfigure);

        // addUniform
        DescriptorSetConfigure &addUniform(const std::function<void(UniformConfigure &)> &configure);

        DescriptorSetConfigure &addUniform(const UniformConfigure &uniformConfigure);

//        DescriptorSetConfigure &addSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

//        DescriptorSetConfigure &addSampler(const std::function<void(VulkanSamplerConfigure &)> &configure);

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

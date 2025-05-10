//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "vklite/device/Device.h"
#include "DescriptorSlotConfigure.h"
#include "vklite/pipeline/descriptor/DescriptorBindingSet.h"
#include "vklite/pipeline/descriptor/uniform/UniformConfigure.h"
#include "vklite/pipeline/descriptor/sampler/SamplerConfigure.h"
#include "vklite/pipeline/descriptor/immutable_sampler/ImmutableSamplerConfigure.h"

namespace vklite {

    class DescriptorSetConfigure {
    private:
        uint32_t mSet;
        // binding -> BindingConfigure
//        std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingConfigure>> mDescriptorBindingConfigures;

    public:

        DescriptorSetConfigure();

        ~DescriptorSetConfigure();

        uint32_t getSet() const;

//        const std::unordered_map<uint32_t, std::unique_ptr<DescriptorBindingConfigure>> &getDescriptorBindingConfigures();

        DescriptorSetConfigure &set(uint32_t set);

        //todo [functional configure]
//        DescriptorSetConfigure &addVulkanDescriptor(std::unique_ptr<VulkanDescriptorBindingConfigure> &&vulkanDescriptor);

//        DescriptorSetConfigure &addUniform(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

        DescriptorSetConfigure &addUniform(const std::function<void(UniformConfigure &)> &configure);

//        DescriptorSetConfigure &addSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

//        DescriptorSetConfigure &addSampler(const std::function<void(VulkanSamplerConfigure &)> &configure);

        //DescriptorSetConfigure &addImmutableSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

//        DescriptorSetConfigure &addImmutableSampler(const std::function<void(VulkanImmutableSamplerConfigure &)> &configure);

//        DescriptorSetConfigure &addStorage(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount = 1);

        std::unique_ptr<DescriptorBindingSet> createVulkanDescriptorBindingSet(const Device &device, const CommandPool &commandPool);

    private:
//        std::vector<vk::DescriptorSetLayoutBinding> createDescriptorSetLayoutBindings();

//        vk::DescriptorSetLayout createDescriptorSetLayout(const VulkanDevice &device);
    };

} // vklite

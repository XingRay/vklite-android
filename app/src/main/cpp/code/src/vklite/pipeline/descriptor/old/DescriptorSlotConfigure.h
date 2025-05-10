//
// Created by leixing on 2025/1/14.
//

#pragma once

//#include <variant>
#include <memory>
#include "vulkan/vulkan.hpp"

#include "vklite/sampler/SamplerInterface.h"

#include "vklite/pipeline/descriptor/old/Descriptor.h"
#include "vklite/pipeline/descriptor/old/DescriptorBufferInfoConfigure.h"
#include "vklite/buffer/UniformBufferBuilder.h"
#include "vklite/pipeline/descriptor/old/DescriptorImageInfoConfigure.h"
#include "vklite/pipeline/descriptor/old/DescriptorImmutableSamplerConfigure.h"

namespace vklite {

    class DescriptorSlotConfigure {
    private:
        uint32_t mBinding;
        vk::DescriptorType mDescriptorType;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;
        vk::ShaderStageFlags mShaderStageFlags;

        std::unique_ptr<DescriptorImmutableSamplerConfigure> mDescriptorImmutableSamplerConfigure;
        std::unique_ptr<DescriptorBufferInfoConfigure> mDescriptorBufferInfoConfigure;
        std::unique_ptr<DescriptorImageInfoConfigure> mDescriptorImageInfoConfigure;

    public:

        DescriptorSlotConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange, vk::ShaderStageFlags shaderStageFlags,
                                         std::unique_ptr<DescriptorBufferInfoConfigure> &&vulkanDescriptorBufferInfoConfigure);

        DescriptorSlotConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange, vk::ShaderStageFlags shaderStageFlags,
                                         std::unique_ptr<DescriptorImageInfoConfigure> &&vulkanDescriptorImageInfoConfigure);

        DescriptorSlotConfigure(uint32_t binding, vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange, vk::ShaderStageFlags shaderStageFlags,
                                         std::unique_ptr<DescriptorImmutableSamplerConfigure> &&vulkanDescriptorImmutableSamplerConfigure);

        ~DescriptorSlotConfigure();

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        vk::DescriptorType getDescriptorType() const;

        [[nodiscard]]
        uint32_t getDescriptorOffset() const;

        [[nodiscard]]
        uint32_t getDescriptorRange() const;

        [[nodiscard]]
        vk::ShaderStageFlags getShaderStageFlags() const;

        [[nodiscard]]
        const std::vector<std::unique_ptr<SamplerInterface>> &getImmutableSamplers() const;

        [[nodiscard]]
        vk::DescriptorSetLayoutBinding createDescriptorSetLayoutBinding() const;

//        [[nodiscard]]
//        std::unique_ptr<VulkanDescriptorBinding> createVulkanDescriptorBinding(const VulkanDevice &device, const CommandPool &commandPool);
    };

} // vklite

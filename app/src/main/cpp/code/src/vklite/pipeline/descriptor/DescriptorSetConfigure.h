//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"

#include "vklite/pipeline/descriptor/uniform_buffer/UniformBufferConfigure.h"
#include "vklite/pipeline/descriptor/combined_image_sampler/CombinedImageSamplerConfigure.h"
#include "vklite/pipeline/descriptor/immutable_sampler/ImmutableSamplerConfigure.h"
#include "vklite/pipeline/descriptor/storage_buffer/StorageBufferConfigure.h"
#include "vklite/pipeline/descriptor/storage_image/StorageImageConfigure.h"

namespace vklite {

    class DescriptorSetConfigure {
    private:
        uint32_t mSet;

        // binding -> BindingConfigure
        std::unordered_map<uint32_t, DescriptorBindingConfigure> mDescriptorBindingConfigures;

    public:

        explicit DescriptorSetConfigure();

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
        DescriptorSetConfigure &addUniformBuffer(const UniformBufferConfigure &configure);

        DescriptorSetConfigure &addUniformBuffer(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addUniformBuffer(uint32_t binding, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addUniformBuffer(const std::function<void(UniformBufferConfigure &)> &configure);


        // combined image sampler
        DescriptorSetConfigure &addCombinedImageSampler(const CombinedImageSamplerConfigure &configure);

        DescriptorSetConfigure &addCombinedImageSampler(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addCombinedImageSampler(uint32_t binding, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addCombinedImageSampler(const std::function<void(CombinedImageSamplerConfigure &)> &configure);


        // immutable sampler
        DescriptorSetConfigure &addImmutableSampler(ImmutableSamplerConfigure &configure);

        DescriptorSetConfigure &addImmutableSampler(uint32_t binding, std::vector<vk::Sampler> &&samplers, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addImmutableSampler(uint32_t binding, vk::Sampler sampler, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addImmutableSampler(const std::function<void(ImmutableSamplerConfigure &)> &configure);


        // storage image
        DescriptorSetConfigure &addStorageImage(const StorageImageConfigure &configure);

        DescriptorSetConfigure &addStorageImage(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addStorageImage(uint32_t binding, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addStorageImage(const std::function<void(StorageImageConfigure &)> &configure);


        //storage buffer
        DescriptorSetConfigure &addStorageBuffer(const StorageBufferConfigure &configure);

        DescriptorSetConfigure &addStorageBuffer(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addStorageBuffer(uint32_t binding, vk::ShaderStageFlags shaderStageFlags);

        DescriptorSetConfigure &addStorageBuffer(const std::function<void(StorageBufferConfigure &)> &configure);

        //todo: addXxx()

        [[nodiscard]]
        std::vector<vk::DescriptorSetLayoutBinding> createDescriptorSetLayoutBindings() const;

    };

} // vklite

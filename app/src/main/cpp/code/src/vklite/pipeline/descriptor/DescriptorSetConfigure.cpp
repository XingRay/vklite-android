//
// Created by leixing on 2025/1/15.
//

#include "DescriptorSetConfigure.h"
#include "vklite/buffer/uniform_buffer/UniformBufferBuilder.h"

namespace vklite {

    DescriptorSetConfigure::DescriptorSetConfigure() : mSet(0) {}

    DescriptorSetConfigure::~DescriptorSetConfigure() = default;


    DescriptorSetConfigure::DescriptorSetConfigure(const DescriptorSetConfigure &other) = default;

    DescriptorSetConfigure &DescriptorSetConfigure::operator=(const DescriptorSetConfigure &other) {
        if (this != &other) {
            mSet = other.mSet;
            mDescriptorBindingConfigures = other.mDescriptorBindingConfigures;
        }
        return *this;
    }

    DescriptorSetConfigure::DescriptorSetConfigure(DescriptorSetConfigure &&other) noexcept
            : mSet(other.mSet),
              mDescriptorBindingConfigures(std::move(other.mDescriptorBindingConfigures)) {}

    DescriptorSetConfigure &DescriptorSetConfigure::operator=(DescriptorSetConfigure &&other) noexcept {
        if (this != &other) {
            mSet = other.mSet;
            mDescriptorBindingConfigures = std::move(other.mDescriptorBindingConfigures);
        }
        return *this;
    }

    uint32_t DescriptorSetConfigure::getSet() const {
        return mSet;
    }

    const std::unordered_map<uint32_t, DescriptorBindingConfigure> &DescriptorSetConfigure::getDescriptorBindingConfigures() const {
        return mDescriptorBindingConfigures;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::set(uint32_t set) {
        mSet = set;
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addDescriptorBinding(DescriptorBindingConfigure &&bindingConfigure) {
        mDescriptorBindingConfigures.emplace(bindingConfigure.getBinding(), bindingConfigure);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addDescriptorBinding(uint32_t binding,
                                                                         vk::DescriptorType descriptorType,
                                                                         uint32_t descriptorCount,
                                                                         vk::ShaderStageFlags shaderStageFlags) {
        DescriptorBindingConfigure descriptorBindingConfigure(binding, descriptorType, descriptorCount, shaderStageFlags);
        addDescriptorBinding(std::move(descriptorBindingConfigure));
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addDescriptorBinding(uint32_t binding,
                                                                         vk::DescriptorType descriptorType,
                                                                         std::vector<vk::Sampler> &&immutableSamplers,
                                                                         vk::ShaderStageFlags shaderStageFlags) {
        DescriptorBindingConfigure descriptorBindingConfigure(binding, descriptorType, std::move(immutableSamplers), shaderStageFlags);
        addDescriptorBinding(std::move(descriptorBindingConfigure));
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addDescriptorBinding(const std::function<void(DescriptorBindingConfigure &)> &configure) {
        DescriptorBindingConfigure bindingConfigure{};
        configure(bindingConfigure);
        addDescriptorBinding(std::move(bindingConfigure));
        return *this;
    }


    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(const UniformConfigure &configure) {
        DescriptorBindingConfigure bindingConfigure = configure.createDescriptorBindingConfigure();
        addDescriptorBinding(std::move(bindingConfigure));
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags) {
        UniformConfigure uniformConfigure(binding, descriptorCount, shaderStageFlags);
        addUniform(uniformConfigure);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(uint32_t binding, vk::ShaderStageFlags shaderStageFlags) {
        addUniform(binding, 1, shaderStageFlags);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(const std::function<void(UniformConfigure &)> &configure) {
        UniformConfigure uniformConfigure{};
        configure(uniformConfigure);
        addUniform(uniformConfigure);
        return *this;
    }


    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(const CombinedImageSamplerConfigure &configure) {
        DescriptorBindingConfigure bindingConfigure = configure.createDescriptorBindingConfigure();
        addDescriptorBinding(std::move(bindingConfigure));
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags) {
        CombinedImageSamplerConfigure samplerConfigure(binding, descriptorCount, shaderStageFlags);
        addSampler(samplerConfigure);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(uint32_t binding, vk::ShaderStageFlags shaderStageFlags) {
        addSampler(binding, 1, shaderStageFlags);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(const std::function<void(CombinedImageSamplerConfigure &)> &configure) {
        CombinedImageSamplerConfigure samplerConfigure{};
        configure(samplerConfigure);
        addSampler(samplerConfigure);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addImmutableSampler(ImmutableSamplerConfigure &configure) {
        DescriptorBindingConfigure bindingConfigure = configure.createDescriptorBindingConfigure();
        addDescriptorBinding(std::move(bindingConfigure));
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addImmutableSampler(uint32_t binding, std::vector<vk::Sampler> &&samplers, vk::ShaderStageFlags shaderStageFlags) {
        ImmutableSamplerConfigure immutableSamplerConfigure(binding, std::move(samplers), shaderStageFlags);
        addImmutableSampler(immutableSamplerConfigure);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addImmutableSampler(const std::function<void(ImmutableSamplerConfigure &)> &configure) {
        ImmutableSamplerConfigure immutableSamplerConfigure{};
        configure(immutableSamplerConfigure);
        addImmutableSampler(immutableSamplerConfigure);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addStorage(const StorageConfigure &configure) {
        DescriptorBindingConfigure bindingConfigure = configure.createDescriptorBindingConfigure();
        addDescriptorBinding(std::move(bindingConfigure));
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addStorage(uint32_t binding, uint32_t descriptorCount, vk::ShaderStageFlags shaderStageFlags) {
        StorageConfigure storageConfigure(binding, descriptorCount, shaderStageFlags);
        addStorage(storageConfigure);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addStorage(const std::function<void(StorageConfigure &)> &configure) {
        StorageConfigure storageConfigure{};
        configure(storageConfigure);
        addStorage(storageConfigure);
        return *this;
    }

    std::vector<vk::DescriptorSetLayoutBinding> DescriptorSetConfigure::createDescriptorSetLayoutBindings() const {
        std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
        descriptorSetLayoutBindings.reserve(mDescriptorBindingConfigures.size());

        for (const auto &entry: mDescriptorBindingConfigures) {
            uint32_t binding = entry.first;
            const DescriptorBindingConfigure &descriptorBindingConfigure = entry.second;

            vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding = descriptorBindingConfigure.createDescriptorSetLayoutBinding();
            descriptorSetLayoutBindings.push_back(descriptorSetLayoutBinding);
        }

        return descriptorSetLayoutBindings;
    }

} // vklite
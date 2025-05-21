//
// Created by leixing on 2025/1/15.
//

#include "DescriptorSetConfigure.h"
#include "vklite/uniform_buffer/UniformBufferBuilder.h"

namespace vklite {

    DescriptorSetConfigure::DescriptorSetConfigure() : mSet(0) {

    }

    DescriptorSetConfigure::~DescriptorSetConfigure() = default;

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

    DescriptorSetConfigure &DescriptorSetConfigure::addDescriptorBinding(const std::function<void(DescriptorBindingConfigure &)> &configure) {
        DescriptorBindingConfigure bindingConfigure{};
        configure(bindingConfigure);
        addDescriptorBinding(std::move(bindingConfigure));
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addDescriptorBinding(DescriptorBindingConfigure &&bindingConfigure) {
        mDescriptorBindingConfigures.emplace(bindingConfigure.getBinding(), bindingConfigure);
        return *this;
    }


    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(const UniformConfigure &configure) {
        DescriptorBindingConfigure bindingConfigure = configure.createDescriptorBindingConfigure();
        addDescriptorBinding(std::move(bindingConfigure));
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(const std::function<void(UniformConfigure &)> &configure) {
        UniformConfigure uniformConfigure{};
        configure(uniformConfigure);
        addUniform(uniformConfigure);
        return *this;
    }


    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(const SamplerConfigure &configure) {
        DescriptorBindingConfigure bindingConfigure = configure.createDescriptorBindingConfigure();
        addDescriptorBinding(std::move(bindingConfigure));
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(const std::function<void(SamplerConfigure &)> &configure) {
        SamplerConfigure samplerConfigure{};
        configure(samplerConfigure);
        addSampler(samplerConfigure);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addImmutableSampler(ImmutableSamplerConfigure &configure) {
        DescriptorBindingConfigure bindingConfigure = configure.createDescriptorBindingConfigure();
        addDescriptorBinding(std::move(bindingConfigure));
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addImmutableSampler(const std::function<void(ImmutableSamplerConfigure &)> &configure) {
        ImmutableSamplerConfigure immutableSamplerConfigure{};
        configure(immutableSamplerConfigure);
        addImmutableSampler(immutableSamplerConfigure);
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
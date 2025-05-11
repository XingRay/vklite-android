//
// Created by leixing on 2025/1/15.
//

#include "DescriptorSetConfigure.h"
#include "vklite/buffer/UniformBufferBuilder.h"

namespace vklite {

    DescriptorSetConfigure::DescriptorSetConfigure() : mSet(0) {

    }

    DescriptorSetConfigure::~DescriptorSetConfigure() = default;

    uint32_t DescriptorSetConfigure::getSet() const {
        return mSet;
    }

    const std::unordered_map<uint32_t, DescriptorBindingConfigure>& DescriptorSetConfigure::getDescriptorBindingConfigures() const{
        return mDescriptorBindingConfigures;
    }

//    const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingConfigure>> &DescriptorSetConfigure::getVulkanDescriptorBindingConfigures() {
//        return mDescriptorBindingConfigures;
//    }

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

    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(const std::function<void(UniformConfigure &)> &configure) {
        UniformConfigure uniformConfigure{};
        configure(uniformConfigure);
        addUniform(uniformConfigure);
        return *this;
    }

    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(const UniformConfigure &uniformConfigure) {
        DescriptorBindingConfigure bindingConfigure = uniformConfigure.createDescriptorBindingConfigure();
        addDescriptorBinding(std::move(bindingConfigure));
        return *this;
    }

    // todo
//    DescriptorSetConfigure &DescriptorSetConfigure::addVulkanDescriptor(std::unique_ptr<VulkanDescriptorBindingConfigure> &&vulkanDescriptor) {
//        mDescriptorBindingConfigures.push_back(std::move(vulkanDescriptor));
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
//        mDescriptorBindingConfigures.push_back(std::make_unique<VulkanDescriptorBindingConfigure>(binding, vk::DescriptorType::eUniformBuffer, descriptorCount, shaderStageFlagBits));
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(const std::function<void(VulkanUniformConfigure &)> &configure) {
//        VulkanUniformConfigure config{};
//        configure(config);
//
//        mDescriptorBindingConfigures[config.mBinding] = std::move(config.createVulkanDescriptorBindingConfigure());
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
//        mDescriptorBindingConfigures.push_back(std::make_unique<VulkanDescriptorBindingConfigure>(binding, vk::DescriptorType::eCombinedImageSampler, descriptorCount, shaderStageFlagBits));
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(const std::function<void(VulkanSamplerConfigure &)> &configure){
//        VulkanSamplerConfigure config{};
//        configure(config);
//
//        mDescriptorBindingConfigures[config.mBinding] = std::move(config.createVulkanDescriptorBindingConfigure());
//        return *this;
//    }
//
//    DescriptorSetConfigure &DescriptorSetConfigure::addImmutableSampler(const std::function<void(VulkanImmutableSamplerConfigure &)> &configure){
//        VulkanImmutableSamplerConfigure config{};
//        configure(config);
//
//        mDescriptorBindingConfigures[config.mBinding] = std::move(config.createVulkanDescriptorBindingConfigure());
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addImmutableSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
//        mDescriptorBindingConfigures.push_back(std::make_unique<VulkanDescriptorBindingConfigure>(binding, vk::DescriptorType::eCombinedImageSampler, descriptorCount, shaderStageFlagBits));
//        return *this;
//    }

//    DescriptorSetConfigure &DescriptorSetConfigure::addStorage(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t descriptorCount) {
//        mDescriptorBindingConfigures.push_back(std::make_unique<VulkanDescriptorBindingConfigure>(binding, vk::DescriptorType::eStorageBuffer, descriptorCount, shaderStageFlagBits));
//        return *this;
//    }


//    DescriptorSetConfigure &DescriptorSetConfigure::addUniform(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, uint32_t size) {
//        VulkanUniformData data{size};
//        mUniformSet.descriptors.emplace_back(binding, shaderStageFlagBits, data);
//        return *this;
//    }
//
//    DescriptorSetConfigure &DescriptorSetConfigure::addSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, ImageSize imageSize) {
//        VulkanSamplerData data{imageSize};
//        mUniformSet.descriptors.emplace_back(binding, shaderStageFlagBits, data);
//        return *this;
//    }
//
//    DescriptorSetConfigure &DescriptorSetConfigure::addAndroidHardwareBufferSampler(uint32_t binding, vk::ShaderStageFlagBits shaderStageFlagBits, AHardwareBuffer *hardwareBuffer) {
//        VulkanAndroidHardwareBufferSamplerData data{hardwareBuffer};
//        mUniformSet.descriptors.emplace_back(binding, shaderStageFlagBits, data);
//        return *this;
//    }
//
//    const VulkanDescriptorSet &DescriptorSetConfigure::build() const {
//        return mUniformSet;
//    }

//    std::unique_ptr<VulkanDescriptorBindingSet> DescriptorSetConfigure::createVulkanDescriptorBindingSet(const VulkanDevice &device, const CommandPool &commandPool) {
//        if (mDescriptorBindingConfigures.empty()) {
//            return nullptr;
//        }
//
//        std::unique_ptr<VulkanDescriptorBindingSet> vulkanDescriptorBindingSet = std::make_unique<VulkanDescriptorBindingSet>();
//
//        for (const auto &entry: mDescriptorBindingConfigures) {
//            uint32_t binding = entry.first;
//            const std::unique_ptr<VulkanDescriptorBindingConfigure> &vulkanDescriptorBindingConfigure = entry.second;
//
//            vulkanDescriptorBindingSet->set(binding, vulkanDescriptorBindingConfigure->createVulkanDescriptorBinding(device, commandPool));
//        }
//
//
//        return vulkanDescriptorBindingSet;
//    }

//    vk::DescriptorSetLayout DescriptorSetConfigure::createDescriptorSetLayout(const VulkanDevice &device) {
//
//        std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings = createDescriptorSetLayoutBindings();
//
//        vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
//        descriptorSetLayoutCreateInfo
//                .setBindings(descriptorSetLayoutBindings);
//
//        return device.getDevice().createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
//    }

    std::vector<vk::DescriptorSetLayoutBinding> DescriptorSetConfigure::createDescriptorSetLayoutBindings() const{
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
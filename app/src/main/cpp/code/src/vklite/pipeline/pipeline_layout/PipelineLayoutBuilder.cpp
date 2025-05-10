//
// Created by leixing on 2025/3/11.
//
#include <iterator>

#include "PipelineLayoutBuilder.h"

#include "vklite/device/Device.h"

namespace vklite {

    PipelineLayoutBuilder::PipelineLayoutBuilder() {
//        mDescriptors = std::make_unique<std::unordered_map<uint32_t, std::unordered_map<uint32_t, Descriptor>>>();
    }

    PipelineLayoutBuilder::~PipelineLayoutBuilder() = default;

    PipelineLayoutBuilder &PipelineLayoutBuilder::addDescriptorSetConfigure(DescriptorSetConfigure &&descriptorSetConfigure) {
        mDescriptorSetConfigures.emplace(descriptorSetConfigure.getSet(), std::move(descriptorSetConfigure));
        return *this;
    }

    PipelineLayoutBuilder &PipelineLayoutBuilder::addDescriptorSetConfigure(std::function<void(DescriptorSetConfigure &)> configure) {
        DescriptorSetConfigure descriptorSetConfigure;
        configure(descriptorSetConfigure);
        addDescriptorSetConfigure(std::move(descriptorSetConfigure));
        return *this;
    }

//    PipelineLayoutConfigure &PipelineLayoutConfigure::addDescriptor(uint32_t set, uint32_t binding, Descriptor &&descriptor) {
//        auto [setIt, setInserted] = (*mDescriptors).try_emplace(set);
//        auto &bindings = setIt->second;
//        bindings.try_emplace(binding, std::move(descriptor));
//        return *this;
//    }

    PipelineLayoutBuilder &PipelineLayoutBuilder::addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags) {
        vk::PushConstantRange pushConstantRange(stageFlags, offset, size);
        mPushConstantRanges.push_back(pushConstantRange);
        return *this;
    }

//    PipelineLayoutConfigure &PipelineLayoutConfigure::addVulkanDescriptorSetConfigure(std::unique_ptr<VulkanDescriptorSetConfigure> &&vulkanDescriptorSetConfigure) {
//        mPipelineLayoutConfigure[vulkanDescriptorSetConfigure->getSet()] = std::move(vulkanDescriptorSetConfigure);
//        return *this;
//    }

//    std::vector<vk::DescriptorSetLayout> PipelineLayoutConfigure::createDescriptorSetLayouts(const VulkanDevice &device) const {
//        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
//
//        for (const auto &entry: mPipelineLayoutConfigure) {
//            uint32_t set = entry.first;
//            const std::unique_ptr<VulkanDescriptorSetConfigure> &descriptorSetConfigure = entry.second;
//
//            std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
//            const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingConfigure>> &vulkanDescriptorBindingConfigures = descriptorSetConfigure->getVulkanDescriptorBindingConfigures();
//            descriptorSetLayoutBindings.reserve(vulkanDescriptorBindingConfigures.size());
//
//            for (const auto &vulkanDescriptorBindingConfiguresEntry: vulkanDescriptorBindingConfigures) {
//                uint32_t binding = vulkanDescriptorBindingConfiguresEntry.first;
//                const std::unique_ptr<VulkanDescriptorBindingConfigure> &vulkanDescriptorConfigure = vulkanDescriptorBindingConfiguresEntry.second;
//
//                vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};
//
//                descriptorSetLayoutBinding
//                        .setBinding(vulkanDescriptorConfigure->getBinding())
//                        .setDescriptorType(vulkanDescriptorConfigure->getDescriptorType())
//                        .setDescriptorCount(vulkanDescriptorConfigure->getDescriptorRange())
//                        .setStageFlags(vulkanDescriptorConfigure->getShaderStageFlags());
//
////                const std::vector<std::unique_ptr<VulkanSampler>> &immutableSamplers = vulkanDescriptorConfigure->getImmutableSamplers();
////                if (!immutableSamplers.empty()) {
////                    std::vector<vk::Sampler> samplers;
////                    for (const std::unique_ptr<VulkanSampler> &vulkanSampler: immutableSamplers) {
////                        samplers.push_back(vulkanSampler->getSampler());
////                    }
////
////                    descriptorSetLayoutBinding
////                            .setDescriptorCount(samplers.size())
////                            .setPImmutableSamplers(samplers.data());
////                }
//
//                descriptorSetLayoutBindings.push_back(descriptorSetLayoutBinding);
//            }
//
//            vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
//            descriptorSetLayoutCreateInfo
//                    .setBindings(descriptorSetLayoutBindings);
//
//            vk::DescriptorSetLayout descriptorSetLayout = device.getDevice().createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
//            descriptorSetLayouts.push_back(descriptorSetLayout);
//        }
//
//        return descriptorSetLayouts;
//    }
//
//    std::vector<vk::DescriptorPoolSize> PipelineLayoutConfigure::createDescriptorPoolSizes(uint32_t frameCount) const {
//        std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;
//        std::unordered_map<vk::DescriptorType, size_t> descriptorTypeToIndexMap;
//
//        for (const auto &vulkanDescriptorSetConfigureEntry: mPipelineLayoutConfigure) {
//            uint32_t set = vulkanDescriptorSetConfigureEntry.first;
//            const std::unique_ptr<VulkanDescriptorSetConfigure> &vulkanDescriptorSetConfigure = vulkanDescriptorSetConfigureEntry.second;
//
//            for (const auto &vulkanDescriptorBindingConfigureEntry: vulkanDescriptorSetConfigure->getVulkanDescriptorBindingConfigures()) {
//                uint32_t binding = vulkanDescriptorBindingConfigureEntry.first;
//                const std::unique_ptr<VulkanDescriptorBindingConfigure> &vulkanDescriptorBindingConfigure = vulkanDescriptorBindingConfigureEntry.second;
//
//                const vk::DescriptorType type = vulkanDescriptorBindingConfigure->getDescriptorType();
//                const uint32_t count = vulkanDescriptorBindingConfigure->getDescriptorRange() * frameCount;
//                if (descriptorTypeToIndexMap.contains(type)) {
//                    descriptorPoolSizes[descriptorTypeToIndexMap[type]].descriptorCount += count;
//                } else {
//                    descriptorTypeToIndexMap[type] = descriptorPoolSizes.size();
//                    descriptorPoolSizes.emplace_back(type, count);
//                }
//            }
//        }
//
//        return descriptorPoolSizes;
//    }
//
//    uint32_t PipelineLayoutConfigure::getSetCount(uint32_t frameCount) const {
//        return mPipelineLayoutConfigure.size() * frameCount;
//    }
//
//    std::unique_ptr<VulkanDescriptorBindingSets> PipelineLayoutConfigure::createVulkanDescriptorBindingSets(const VulkanDevice &device, const CommandPool &commandPool) const {
//        if (mPipelineLayoutConfigure.empty()) {
//            return nullptr;
//        }
//
//        std::unique_ptr<VulkanDescriptorBindingSets> vulkanDescriptorBindingSets = std::make_unique<VulkanDescriptorBindingSets>();
//
//        for (const auto &entry: mPipelineLayoutConfigure) {
//            uint32_t set = entry.first;
//            const std::unique_ptr<VulkanDescriptorSetConfigure> &vulkanDescriptorSetConfigure = entry.second;
//
//            std::unique_ptr<VulkanDescriptorBindingSet> vulkanDescriptorBindingSet = vulkanDescriptorSetConfigure->createVulkanDescriptorBindingSet(device, commandPool);
//            vulkanDescriptorBindingSets->set(set, std::move(vulkanDescriptorBindingSet));
//        }
//
//        return vulkanDescriptorBindingSets;
//    }

    std::vector<std::vector<vk::DescriptorSetLayoutBinding>> PipelineLayoutBuilder::createDescriptorSetLayoutBindings() {
        std::vector<std::vector<vk::DescriptorSetLayoutBinding>> descriptorSetLayoutBindingSets;
        for (const auto &entry: mDescriptorSetConfigures) {
            uint32_t set = entry.first;
            const DescriptorSetConfigure &descriptorSetConfigure = entry.second;

            std::vector<vk::DescriptorSetLayoutBinding> bindingSet = descriptorSetConfigure.createDescriptorSetLayoutBindings();
        }
        //=mDescriptorSetConfigures.createDescriptorSetLayoutBindings();
        return descriptorSetLayoutBindingSets;
    }

    PipelineLayout PipelineLayoutBuilder::build(const Device &device) {
        std::vector<std::vector<vk::DescriptorSetLayoutBinding>> descriptorSetLayoutBindingSets = createDescriptorSetLayoutBindings();
        return PipelineLayout(device, descriptorSetLayoutBindingSets, mPushConstantRanges);
    }

    std::unique_ptr<PipelineLayout> PipelineLayoutBuilder::buildUnique(const Device &device) {
        std::vector<std::vector<vk::DescriptorSetLayoutBinding>> descriptorSetLayoutBindingSets = createDescriptorSetLayoutBindings();
        return std::make_unique<PipelineLayout>(device, descriptorSetLayoutBindingSets, mPushConstantRanges);
    }


} // vklite
//
// Created by leixing on 2025/3/11.
//
#include <iterator>

#include "DescriptorConfigure.h"

#include "vklite/device/Device.h"

namespace vklite {

    DescriptorConfigure::DescriptorConfigure() {
//        mDescriptors = std::make_unique<std::unordered_map<uint32_t, std::unordered_map<uint32_t, Descriptor>>>();
    }

    DescriptorConfigure::~DescriptorConfigure() = default;

    DescriptorConfigure &DescriptorConfigure::addDescriptorSetConfigure(DescriptorSetConfigure &&descriptorSetConfigure) {
        mDescriptorSetConfigures.emplace(descriptorSetConfigure.getSet(), std::move(descriptorSetConfigure));
        return *this;
    }

    DescriptorConfigure &DescriptorConfigure::addDescriptorSetConfigure(std::function<void(DescriptorSetConfigure &)> configure) {
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

    uint32_t DescriptorConfigure::getDescriptorSetCount() const {
        return mDescriptorSetConfigures.size();
    }

    std::vector<vk::DescriptorPoolSize> DescriptorConfigure::calcDescriptorPoolSizes() const {
        std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;
        std::unordered_map<vk::DescriptorType, size_t> descriptorTypeToIndexMap;

        for (const auto &setEntry: mDescriptorSetConfigures) {
            uint32_t set = setEntry.first;
            const DescriptorSetConfigure &descriptorSet = setEntry.second;
            const std::unordered_map<uint32_t, DescriptorBindingConfigure> &descriptorBindingConfigures = descriptorSet.getDescriptorBindingConfigures();

            for (const auto &bindingEntry: descriptorBindingConfigures) {
                uint32_t binding = bindingEntry.first;
                const DescriptorBindingConfigure &descriptor = bindingEntry.second;

                const vk::DescriptorType type = descriptor.getDescriptorType();
                const uint32_t count = descriptor.getDescriptorCount();
                if (descriptorTypeToIndexMap.contains(type)) {
                    descriptorPoolSizes[descriptorTypeToIndexMap[type]].descriptorCount += count;
                } else {
                    descriptorTypeToIndexMap[type] = descriptorPoolSizes.size();
                    descriptorPoolSizes.emplace_back(type, count);
                }
            }
        }

        return descriptorPoolSizes;
    }

    std::vector<vk::DescriptorSetLayout> DescriptorConfigure::createDescriptorSetLayouts(const Device &device) const {

        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;

        for (const auto &setEntry: mDescriptorSetConfigures) {
            uint32_t set = setEntry.first;
            const DescriptorSetConfigure &descriptorSetConfigure = setEntry.second;

            std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings = descriptorSetConfigure.createDescriptorSetLayoutBindings();

            vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
            descriptorSetLayoutCreateInfo
                    .setBindings(descriptorSetLayoutBindings);

            vk::DescriptorSetLayout descriptorSetLayout = device.getDevice().createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
            descriptorSetLayouts.push_back(descriptorSetLayout);
        }

        return descriptorSetLayouts;
    }

} // vklite
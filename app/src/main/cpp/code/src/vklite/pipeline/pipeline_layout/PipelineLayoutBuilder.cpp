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
    uint32_t PipelineLayoutBuilder::getDescriptorSetCount() const {
        return mDescriptorSetConfigures.size();
    }

    std::vector<vk::DescriptorPoolSize> PipelineLayoutBuilder::calcDescriptorPoolSizes() const {
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

    std::unique_ptr<DescriptorPool> PipelineLayoutBuilder::createDescriptorPool(const Device &device, uint32_t frameCount) const {
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
                const uint32_t count = descriptor.getDescriptorCount() * frameCount;
                if (descriptorTypeToIndexMap.contains(type)) {
                    descriptorPoolSizes[descriptorTypeToIndexMap[type]].descriptorCount += count;
                } else {
                    descriptorTypeToIndexMap[type] = descriptorPoolSizes.size();
                    descriptorPoolSizes.emplace_back(type, count);
                }
            }
        }

        return std::make_unique<DescriptorPool>(device, descriptorPoolSizes, mDescriptorSetConfigures.size() * frameCount);
    }

    std::vector<vk::DescriptorSetLayout> PipelineLayoutBuilder::createDescriptorSetLayouts(const Device &device) const {

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

    uint32_t PipelineLayoutBuilder::calcTotalPushConstantSize() const {
        uint32_t totalPushConstantSize = 0;
        for (const vk::PushConstantRange &pushConstantRange: mPushConstantRanges) {
            totalPushConstantSize += pushConstantRange.size;
        }
        return totalPushConstantSize;
    }

    PipelineLayout PipelineLayoutBuilder::buildPipelineLayout(const Device &device) const {
        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = createDescriptorSetLayouts(device);
//        return PipelineLayout(device, descriptorSetLayoutBindingSets, mPushConstantRanges);
        return {device, descriptorSetLayouts, mPushConstantRanges};
    }

    std::unique_ptr<PipelineLayout> PipelineLayoutBuilder::buildUniquePipelineLayout(const Device &device) const {
        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = createDescriptorSetLayouts(device);
        return std::make_unique<PipelineLayout>(device, descriptorSetLayouts, mPushConstantRanges);
    }

} // vklite
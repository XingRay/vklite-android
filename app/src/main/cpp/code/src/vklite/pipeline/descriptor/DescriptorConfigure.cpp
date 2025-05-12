//
// Created by leixing on 2025/3/11.
//
#include <iterator>

#include "DescriptorConfigure.h"

#include "vklite/device/Device.h"

namespace vklite {

    DescriptorConfigure::DescriptorConfigure() = default;

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
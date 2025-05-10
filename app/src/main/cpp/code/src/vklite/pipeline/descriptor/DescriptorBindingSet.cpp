//
// Created by leixing on 2025/3/28.
//

#include "DescriptorBindingSet.h"

namespace vklite {

    DescriptorBindingSet::DescriptorBindingSet() = default;

    DescriptorBindingSet::~DescriptorBindingSet() = default;

//    const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBinding>> &VulkanDescriptorBindingSet::getVulkanDescriptorBindings() const {
//        return mDescriptorBindings;
//    }
//
//    VulkanDescriptorBindingSet &VulkanDescriptorBindingSet::set(uint32_t binding, std::unique_ptr<VulkanDescriptorBinding> &&vulkanDescriptorBinding) {
//        mDescriptorBindings[binding] = std::move(vulkanDescriptorBinding);
//        return *this;
//    }
//
//    VulkanDescriptorBindingSet &VulkanDescriptorBindingSet::updateUniformBuffer(const CommandPool& commandPool, uint32_t binding, const void *data, uint32_t size) {
//        mDescriptorBindings[binding]->updateUniformBuffer(commandPool, data, size);
//        return *this;
//    }

} // vklite
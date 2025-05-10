//
// Created by leixing on 2025/3/28.
//

#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>

#include "vklite/pipeline/descriptor/old/DescriptorBindingSet.h"

namespace vklite {

    class DescriptorBindingSets {
    private:
        // set -> VulkanDescriptorBindingSet
//        std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingSet>> mDescriptorBindingSets;

    public:
        DescriptorBindingSets();

        ~DescriptorBindingSets();

//        [[nodiscard]]
//        const std::unordered_map<uint32_t, std::unique_ptr<VulkanDescriptorBindingSet>> &getVulkanDescriptorBindingSets() const;
//
//        VulkanDescriptorBindingSets &set(uint32_t set, std::unique_ptr<VulkanDescriptorBindingSet> &&vulkanDescriptorBindingSet);
//
//        VulkanDescriptorBindingSets &updateUniformBuffer(const CommandPool& commandPool, uint32_t set, uint32_t binding, const void *data, uint32_t size);

    };

} // vklite

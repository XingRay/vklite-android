//
// Created by leixing on 2025/3/12.
//

#pragma once

#include <memory>

#include "vklite/device/Device.h"

namespace vklite {

    class DescriptorPool {
    private:
        const Device &mVulkanDevice;

        vk::DescriptorPool mDescriptorPool;
    public:
        DescriptorPool(const Device &device, std::vector<vk::DescriptorPoolSize> descriptorPoolSizes, uint32_t maxSets);

        ~DescriptorPool();

        [[nodiscard]]
        const vk::DescriptorPool &getDescriptorPool() const;

        std::vector<vk::DescriptorSet> allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts);
    };

} // vklite

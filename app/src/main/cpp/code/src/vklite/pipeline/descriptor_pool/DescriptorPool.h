//
// Created by leixing on 2025/3/12.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class DescriptorPool {
    private:
        vk::Device mDevice;
        vk::DescriptorPool mDescriptorPool;

    public:
        explicit DescriptorPool(vk::Device device, vk::DescriptorPool descriptorPool);

        ~DescriptorPool();

        DescriptorPool(const DescriptorPool &other) = delete;

        DescriptorPool &operator=(const DescriptorPool &other) = delete;

        DescriptorPool(DescriptorPool &&other) noexcept;

        DescriptorPool &operator=(DescriptorPool &&other) noexcept;

        [[nodiscard]]
        const vk::DescriptorPool &getVkDescriptorPool() const;

        [[nodiscard]]
        std::vector<vk::DescriptorSet> allocateDescriptorSets(const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts);
    };

} // vklite

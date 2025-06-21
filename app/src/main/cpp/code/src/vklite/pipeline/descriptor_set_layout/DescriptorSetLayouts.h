//
// Created by leixing on 2025/6/5.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class DescriptorSetLayouts {
    private:
        vk::Device mDevice;
        std::vector<vk::DescriptorSetLayout> mDescriptorSetLayouts;

    public:
        explicit DescriptorSetLayouts(vk::Device device, std::vector<vk::DescriptorSetLayout> &&descriptorSetLayouts);

        ~DescriptorSetLayouts();

        DescriptorSetLayouts(const DescriptorSetLayouts &other) = delete;

        DescriptorSetLayouts &operator=(const DescriptorSetLayouts &other) = delete;

        DescriptorSetLayouts(DescriptorSetLayouts &&other) noexcept;

        DescriptorSetLayouts &operator=(DescriptorSetLayouts &&other) noexcept;

        [[nodiscard]]
        const std::vector<vk::DescriptorSetLayout> &getDescriptorSetLayouts() const;
    };

} // vklite

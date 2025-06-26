//
// Created by leixing on 2025-06-19.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class DescriptorSetLayout {
    private:
        vk::Device mDevice;
        vk::DescriptorSetLayout mDescriptorSetLayout;

    public:
        explicit DescriptorSetLayout(vk::Device device, vk::DescriptorSetLayout descriptorSetLayout);

        ~DescriptorSetLayout();

        DescriptorSetLayout(const DescriptorSetLayout &other) = delete;

        DescriptorSetLayout &operator=(const DescriptorSetLayout &other) = delete;

        DescriptorSetLayout(DescriptorSetLayout &&other) noexcept;

        DescriptorSetLayout &operator=(DescriptorSetLayout &&other) noexcept;

        [[nodiscard]]
        const vk::DescriptorSetLayout &getVkDescriptorSetLayout() const;
    };

} // vklite

//
// Created by leixing on 2024/12/26.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "vklite/swapchain/Swapchain.h"
#include "vklite/Log.h"

namespace vklite {
    class VulkanRenderPass {
    private://fields
        const Device& mDevice;
        vk::RenderPass mRenderPass;

    public://methods
        VulkanRenderPass(const Device &vulkanDevice, const Swapchain &vulkanSwapchain);

        ~VulkanRenderPass();

        [[nodiscard]]
        const vk::RenderPass &getRenderPass() const;

        static vk::Format findDepthFormat(const vk::PhysicalDevice &physicalDevice);

        static vk::Format findSupportedFormat(const vk::PhysicalDevice &physicalDevice,
                                              const std::vector<vk::Format> &candidates,
                                              vk::ImageTiling tiling,
                                              vk::FormatFeatureFlags features);
    };
}

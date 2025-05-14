//
// Created by leixing on 2024/12/26.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "vklite/swapchain/Swapchain.h"
#include "vklite/Log.h"

namespace vklite {

    class RenderPass {

    private://fields
        const Device &mDevice;
        vk::RenderPass mRenderPass;

    public://methods
        RenderPass(const Device &device,
                   const std::vector<vk::AttachmentDescription> &attachmentDescriptions,
                   const std::vector<vk::SubpassDescription> &subpassDescriptions,
                   const std::vector<vk::SubpassDependency> &subpassDependencies);

        ~RenderPass();

        [[nodiscard]]
        const vk::RenderPass &getRenderPass() const;

        static vk::Format findDepthFormat(const vk::PhysicalDevice &physicalDevice);

        static vk::Format findSupportedFormat(const vk::PhysicalDevice &physicalDevice,
                                              const std::vector<vk::Format> &candidates,
                                              vk::ImageTiling tiling,
                                              vk::FormatFeatureFlags features);
    };
}

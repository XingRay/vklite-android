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

        std::vector<vk::ClearValue> mClearValues;
        vk::RenderPassBeginInfo mBeginInfo;
        vk::SubpassContents mSubpassContents;

    public://methods
        RenderPass(const Device &device,
                   const std::vector<vk::AttachmentDescription> &attachmentDescriptions,
                   const std::vector<vk::SubpassDescription> &subpassDescriptions,
                   const std::vector<vk::SubpassDependency> &subpassDependencies,
                   std::vector<vk::ClearValue> &&clearValues,
                   vk::Rect2D renderArea,
                   vk::SubpassContents subpassContents);

        ~RenderPass();

        [[nodiscard]]
        const vk::RenderPass &getRenderPass() const;

        RenderPass &renderArea(vk::Rect2D renderArea);

        RenderPass &renderArea(int32_t x, int32_t y, uint32_t width, uint32_t height);

        RenderPass &renderAreaOffset(vk::Offset2D offset);

        RenderPass &renderAreaOffset(int32_t x, int32_t y);

        RenderPass &renderAreaExtend(vk::Extent2D extent);

        RenderPass &renderAreaExtend(uint32_t width, uint32_t height);

        RenderPass &subpassContents(vk::SubpassContents subpassContents);

        RenderPass &clearValues(std::vector<vk::ClearValue> clearValues);

        RenderPass &clearValue(size_t index, vk::ClearValue clearValue);

        void execute(const vk::CommandBuffer &, const vk::Framebuffer &framebuffer,
                     std::function<void(const vk::CommandBuffer &commandBuffer)> handler);
    };
}

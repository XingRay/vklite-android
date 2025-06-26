//
// Created by leixing on 2024/12/26.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/render_pass/RenderPassBeginInfo.h"
#include "vklite/Log.h"

namespace vklite {

    class RenderPass {

    private://fields
        vk::Device mDevice;
        vk::RenderPass mRenderPass;
        RenderPassBeginInfo mBeginInfo;

    public://methods
        explicit RenderPass(const vk::Device &device, const vk::RenderPass &renderPass, RenderPassBeginInfo &&beginInfo);

        ~RenderPass();

        RenderPass(const RenderPass &other) = delete;

        RenderPass &operator=(const RenderPass &other) = delete;

        RenderPass(RenderPass &&other) noexcept;

        RenderPass &operator=(RenderPass &&other) noexcept;

        [[nodiscard]]
        const vk::RenderPass &getVkRenderPass() const;

        RenderPass &renderArea(vk::Rect2D renderArea);

        RenderPass &renderArea(int32_t x, int32_t y, uint32_t width, uint32_t height);

        RenderPass &renderAreaOffset(vk::Offset2D offset);

        RenderPass &renderAreaOffset(int32_t x, int32_t y);

        RenderPass &renderAreaExtend(vk::Extent2D extent);

        RenderPass &renderAreaExtend(uint32_t width, uint32_t height);

        RenderPass &subpassContents(vk::SubpassContents subpassContents);

        RenderPass &clearValues(std::vector<vk::ClearValue> &&clearValues);

        RenderPass &updateClearValue(size_t index, vk::ClearValue clearValue);

        void execute(const vk::CommandBuffer &, const vk::Framebuffer &framebuffer,
                     std::function<void(const vk::CommandBuffer &commandBuffer)> handler);
    };
}

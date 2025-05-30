//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>
#include <functional>

#include "vklite/render_pass/RenderPass.h"
#include "vklite/render_pass/subpass/Subpass.h"
#include "vklite/render_pass/attachment/Attachment.h"
#include "vklite/render_pass/RenderPassBeginInfo.h"

namespace vklite {

    class RenderPassBuilder {
    private:
        vk::Device mDevice;
        vk::RenderPassCreateInfo mRenderPassCreateInfo;

        std::vector<Subpass> mSubpasses;
        std::vector<Attachment> mAttachments;

        bool mAddAttachmentInvoked = false;

        RenderPassBeginInfo mRenderPassBeginInfo;

    public:
        RenderPassBuilder();

        ~RenderPassBuilder();

        RenderPassBuilder &device(vk::Device device);

        RenderPassBuilder &flags(vk::RenderPassCreateFlags flags);

        RenderPassBuilder &addSubpass(const std::function<void(Subpass &subpass, const std::vector<Subpass> &subpasses)> &configure);

        RenderPassBuilder &addAttachment(const std::function<void(Attachment &attachment, std::vector<Subpass> &subpasses)> &configure);

        RenderPassBuilder &addAttachmentIf(bool condition, const std::function<void(Attachment &attachment, std::vector<Subpass> &subpasses)> &configure);

        RenderPassBuilder &renderArea(vk::Rect2D renderArea);

        RenderPassBuilder &renderArea(int32_t x, int32_t y, uint32_t width, uint32_t height);

        RenderPassBuilder &renderAreaOffset(vk::Offset2D offset);

        RenderPassBuilder &renderAreaOffset(int32_t x, int32_t y);

        RenderPassBuilder &renderAreaExtend(vk::Extent2D extent);

        RenderPassBuilder &renderAreaExtend(uint32_t width, uint32_t height);

        RenderPassBuilder &subpassContents(vk::SubpassContents subpassContents);

        RenderPass build();

        std::unique_ptr<RenderPass> buildUnique();
    };

} // vklite

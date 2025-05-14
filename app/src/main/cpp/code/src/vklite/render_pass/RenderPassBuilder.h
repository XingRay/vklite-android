//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>
#include <functional>

#include "vklite/render_pass/RenderPass.h"
#include "vklite/render_pass/subpass/Subpass.h"
#include "vklite/render_pass/attachment/Attachment.h"

namespace vklite {

    class RenderPassBuilder {
    private:
        std::vector<Subpass> mSubpasses;
        std::vector<Attachment> mAttachments;

        bool mAddAttachmentInvoked = false;

    public:
        RenderPassBuilder();

        ~RenderPassBuilder();

        RenderPassBuilder &addSubpass(const std::function<void(Subpass &subpass, const std::vector<Subpass> &subpasses)> &configure);

        RenderPassBuilder &addAttachment(const std::function<void(Attachment &attachment, std::vector<Subpass> &subpasses)> &configure);

        RenderPassBuilder &addAttachmentIf(bool condition, const std::function<void(Attachment &attachment, std::vector<Subpass> &subpasses)> &configure);

        std::unique_ptr<RenderPass> buildUnique(const Device &device);

        RenderPass build(const Device &device);
    };

} // vklite

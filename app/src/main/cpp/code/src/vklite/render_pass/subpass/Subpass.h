//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <cstdint>
#include <functional>

#include <vulkan/vulkan.hpp>
#include "vklite/render_pass/subpass/SubpassDescription.h"
#include "vklite/render_pass/attachment/AttachmentReference.h"

namespace vklite {

    class Subpass {
    private:
        // basic
        vk::SubpassDescriptionFlags mFlags;
        vk::PipelineBindPoint mPipelineBindPoint;

        // attachments
        std::vector<AttachmentReference> mInputAttachments;
        std::vector<AttachmentReference> mColorAttachments;
        std::vector<AttachmentReference> mResolveAttachments;
        std::optional<AttachmentReference> mDepthStencilAttachment;
        std::vector<AttachmentReference> mPreserveAttachments;

        // as dependency
        uint32_t mIndex;
        vk::PipelineStageFlags mStageMask;
        vk::AccessFlags mAccessMask;
        // dependencies
        std::vector<std::reference_wrapper<const Subpass>> mDependencies;

    public:
        Subpass();

        ~Subpass();

        Subpass &flags(vk::SubpassDescriptionFlags flags);

        Subpass &pipelineBindPoint(vk::PipelineBindPoint pipelineBindPoint);

        Subpass &index(uint32_t index);

        Subpass &stageMask(vk::PipelineStageFlags stageMask);

        Subpass &accessMask(vk::AccessFlags accessMask);

        Subpass &addDependency(const Subpass &srcSubpass);

        Subpass &addInputAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &addColorAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &addResolveAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &setDepthStencilAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &addPreserveAttachment(const Attachment &attachment, vk::ImageLayout layout);

        [[nodiscard]]
        SubpassDescription createSubpassDescription() const;

        [[nodiscard]]
        std::vector<vk::SubpassDependency> createSubpassDependencies() const;

    public://static
        static Subpass externalSubpass();
    };

} // vklite

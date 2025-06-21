//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <cstdint>
#include <functional>

#include <vulkan/vulkan.hpp>
#include "vklite/render_pass/subpass/SubpassDescription.h"
#include "vklite/render_pass/attachment/AttachmentReference.h"
#include "vklite/render_pass/subpass/SubpassDependency.h"

namespace vklite {

    class Subpass {
    private:
        uint32_t mIndex;

        // basic
        vk::SubpassDescriptionFlags mFlags;
        vk::PipelineBindPoint mPipelineBindPoint;

        // attachments
        std::vector<AttachmentReference> mInputAttachments;
        std::vector<AttachmentReference> mColorAttachments;
        std::vector<AttachmentReference> mResolveAttachments;
        std::optional<AttachmentReference> mDepthStencilAttachment;
        std::vector<AttachmentReference> mPreserveAttachments;

        // dependencies
        std::vector<SubpassDependency> mDependencies;

    public:
        explicit Subpass();

        ~Subpass();

        Subpass &flags(vk::SubpassDescriptionFlags flags);

        Subpass &pipelineBindPoint(vk::PipelineBindPoint pipelineBindPoint);

        Subpass &index(uint32_t index);

        Subpass &addDependency(const Subpass &srcSubpass, vk::PipelineStageFlags srcStage, vk::AccessFlags srcAccess,
                               vk::PipelineStageFlags dstStage, vk::AccessFlags dstAccess, vk::DependencyFlags flags = vk::DependencyFlags{});

        Subpass &addInputAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &addColorAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &addResolveAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &setDepthStencilAttachment(const Attachment &attachment, vk::ImageLayout layout);

        Subpass &addPreserveAttachment(const Attachment &attachment, vk::ImageLayout layout);

        [[nodiscard]]
        uint32_t getIndex() const;

        [[nodiscard]]
        SubpassDescription createSubpassDescription() const;

        [[nodiscard]]
        std::vector<vk::SubpassDependency> createSubpassDependencies() const;

    public://static
        static Subpass externalSubpass();
    };

} // vklite

//
// Created by leixing on 2025/5/14.
//

#include "SubpassDescription.h"

namespace vklite {

    SubpassDescription::SubpassDescription(
            vk::SubpassDescriptionFlags flags,
            vk::PipelineBindPoint pipelineBindPoint,
            std::vector<vk::AttachmentReference> inputAttachments,
            std::vector<vk::AttachmentReference> colorAttachments,
            std::vector<vk::AttachmentReference> resolveAttachments,
            std::optional<vk::AttachmentReference> depthStencilAttachment,
            std::vector<uint32_t> preserveAttachments)
            : mFlags(flags),
              mPipelineBindPoint(pipelineBindPoint),
              mInputAttachments(std::move(inputAttachments)),
              mColorAttachments(std::move(colorAttachments)),
              mResolveAttachments(std::move(resolveAttachments)),
              mDepthStencilAttachment(depthStencilAttachment),
              mPreserveAttachments(std::move(preserveAttachments)) {}

    SubpassDescription::~SubpassDescription() = default;

    vk::SubpassDescription SubpassDescription::createSubpassDescription() const {
        vk::SubpassDescription subpassDescription{};
        subpassDescription
                .setFlags(mFlags)
                .setPipelineBindPoint(mPipelineBindPoint)
                .setInputAttachments(mInputAttachments)
                .setPreserveAttachments(mPreserveAttachments);

        if (!mColorAttachments.empty()) {
            subpassDescription.setColorAttachments(mColorAttachments);
        }
        if (!mResolveAttachments.empty()) {
            // ** setResolveAttachments 会同时设置 colorAttachmentCount, 而不存在 resolveAttachmentCount
            subpassDescription.setResolveAttachments(mResolveAttachments);
        }

        if (mDepthStencilAttachment.has_value()) {
            subpassDescription.setPDepthStencilAttachment(&(mDepthStencilAttachment.value()));
        }

        return subpassDescription;
    }
} // vklite
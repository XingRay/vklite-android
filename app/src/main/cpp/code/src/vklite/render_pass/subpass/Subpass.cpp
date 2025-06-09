//
// Created by leixing on 2025/5/13.
//

#include "Subpass.h"

namespace vklite {

    Subpass::Subpass()
            : mFlags(vk::SubpassDescriptionFlags{}),
              mPipelineBindPoint(vk::PipelineBindPoint::eGraphics) {}

    Subpass::~Subpass() = default;

    Subpass &Subpass::flags(vk::SubpassDescriptionFlags flags) {
        mFlags = flags;
        return *this;
    }

    Subpass &Subpass::pipelineBindPoint(vk::PipelineBindPoint pipelineBindPoint) {
        mPipelineBindPoint = pipelineBindPoint;
        return *this;
    }

    Subpass &Subpass::index(uint32_t index) {
        mIndex = index;
        return *this;
    }

    Subpass &Subpass::stageMask(vk::PipelineStageFlags stageMask) {
        mStageMask = stageMask;
        return *this;
    }

    Subpass &Subpass::accessMask(vk::AccessFlags accessMask) {
        mAccessMask = accessMask;
        return *this;
    }

    Subpass &Subpass::addDependency(const Subpass &srcSubpass) {
        mDependencies.push_back(std::cref(srcSubpass));
        return *this;
    }

    Subpass &Subpass::addInputAttachment(const Attachment &attachment, vk::ImageLayout layout) {
        mInputAttachments.push_back(std::move(AttachmentReference(attachment, layout)));
        return *this;
    }

    Subpass &Subpass::addColorAttachment(const Attachment &attachment, vk::ImageLayout layout) {
        mColorAttachments.push_back(std::move(AttachmentReference(attachment, layout)));
        return *this;
    }

    Subpass &Subpass::addResolveAttachment(const Attachment &attachment, vk::ImageLayout layout) {
        mResolveAttachments.push_back(std::move(AttachmentReference(attachment, layout)));
        return *this;
    }

    Subpass &Subpass::setDepthStencilAttachment(const Attachment &attachment, vk::ImageLayout layout) {
        mDepthStencilAttachment = std::move(AttachmentReference(attachment, layout));
        return *this;
    }

    Subpass &Subpass::addPreserveAttachment(const Attachment &attachment, vk::ImageLayout layout) {
        mPreserveAttachments.push_back(std::move(AttachmentReference(attachment, layout)));
        return *this;
    }

    std::vector<vk::AttachmentReference> convertAttachments(const std::vector<AttachmentReference> &attachmentReferences) {
        std::vector<vk::AttachmentReference> vkAttachmentReferences;
        vkAttachmentReferences.reserve(attachmentReferences.size());
        for (const AttachmentReference &attachmentReference: attachmentReferences) {
            vkAttachmentReferences.push_back(attachmentReference.createAttachmentReference());
        }
        return vkAttachmentReferences;
    }

    std::optional<vk::AttachmentReference> convertDepthStencilAttachment(const std::optional<AttachmentReference> &attachmentReference) {
        if (attachmentReference.has_value()) {
            return attachmentReference.value().createAttachmentReference();
        } else {
            return std::nullopt;
        }
    }

    std::vector<uint32_t> convertPreserveAttachments(const std::vector<AttachmentReference> &attachmentReferences) {
        std::vector<uint32_t> attachmentIndices;
        attachmentIndices.reserve(attachmentReferences.size());
        for (const AttachmentReference &attachmentReference: attachmentReferences) {
            attachmentIndices.push_back(attachmentReference.getIndex());
        }
        return attachmentIndices;
    }

    SubpassDescription Subpass::createSubpassDescription() const {
        return SubpassDescription{
                mFlags,
                mPipelineBindPoint,
                convertAttachments(mInputAttachments),
                convertAttachments(mColorAttachments),
                convertAttachments(mResolveAttachments),
                convertDepthStencilAttachment(mDepthStencilAttachment),
                convertPreserveAttachments(mPreserveAttachments)
        };
    }

    std::vector<vk::SubpassDependency> Subpass::createSubpassDependencies() const {
        std::vector<vk::SubpassDependency> subpassDependencies;
        subpassDependencies.reserve(mDependencies.size());
        for (const std::reference_wrapper<const Subpass> &subpass: mDependencies) {
            vk::SubpassDependency dependency = vk::SubpassDependency{}
                    .setSrcSubpass(subpass.get().mIndex)
                    .setSrcStageMask(subpass.get().mStageMask)
                    .setSrcAccessMask(subpass.get().mAccessMask)
                    .setDstSubpass(mIndex)
                    .setDstStageMask(mStageMask)
                    .setDstAccessMask(mAccessMask);
            subpassDependencies.push_back(dependency);
        }

        return subpassDependencies;
    }

    Subpass Subpass::externalSubpass() {
        Subpass externalSubpass{};
        externalSubpass
                .index(vk::SubpassExternal)
                .stageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
                .accessMask(vk::AccessFlagBits::eNone);
        return externalSubpass;
    }

} // vklite
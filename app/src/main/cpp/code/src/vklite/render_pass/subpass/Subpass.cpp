//
// Created by leixing on 2025/5/13.
//

#include "Subpass.h"

#include <format>

namespace vklite {

    Subpass::Subpass()
            : mIndex(0),
              mPipelineBindPoint(vk::PipelineBindPoint::eGraphics),
              mFlags(vk::SubpassDescriptionFlags{}) {}

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

    Subpass &Subpass::addDependency(const Subpass &srcSubpass, vk::PipelineStageFlags srcStage, vk::AccessFlags srcAccess,
                                    vk::PipelineStageFlags dstStage, vk::AccessFlags dstAccess, vk::DependencyFlags flags) {
        if (srcSubpass.mIndex != vk::SubpassExternal && srcSubpass.mIndex >= mIndex) {
            throw std::runtime_error(std::format("srcSubpass.mIndex({}) >= mIndex({})", srcSubpass.mIndex, mIndex));
        }

        mDependencies.emplace_back(SubpassDependencyItem(srcSubpass.mIndex, srcStage, srcAccess),
                                   SubpassDependencyItem(mIndex, dstStage, dstAccess),
                                   flags);
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

    uint32_t Subpass::getIndex() const {
        return mIndex;
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
        for (const SubpassDependency &subpassDependency: mDependencies) {
            subpassDependencies.emplace_back(subpassDependency.createSubpassDependency());
        }

        return subpassDependencies;
    }

    Subpass Subpass::externalSubpass() {
        Subpass externalSubpass{};
        externalSubpass
                .index(vk::SubpassExternal);
        return externalSubpass;
    }

} // vklite
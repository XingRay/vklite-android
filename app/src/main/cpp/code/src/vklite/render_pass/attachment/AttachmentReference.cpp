//
// Created by leixing on 2025/5/13.
//

#include "AttachmentReference.h"

namespace vklite {

    AttachmentReference::AttachmentReference(const Attachment &attachment, vk::ImageLayout layout)
            : mAttachment(std::cref(attachment)), mLayout(layout) {}

    AttachmentReference::~AttachmentReference() = default;

    uint32_t AttachmentReference::getIndex() const {
        return mAttachment.get().getIndex();
    }

    vk::AttachmentReference AttachmentReference::createAttachmentReference() const {
        return vk::AttachmentReference{}
                .setAttachment(mAttachment.get().getIndex())
                .setLayout(mLayout);
    }

} // vklite
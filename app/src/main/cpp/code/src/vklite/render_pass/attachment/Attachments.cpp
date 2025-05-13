//
// Created by leixing on 2025/5/13.
//

#include "Attachments.h"

namespace vklite {

    Attachments::Attachments() = default;

    Attachments::~Attachments() = default;

    Attachments Attachments::addAttachment(Attachment &&attachment) {
        mAttachments.push_back(attachment);
        return *this;
    }

    Attachments Attachments::addAttachment(const std::function<Attachment()> &attachment) {
        mAttachments.push_back(attachment());
        return *this;
    }

    Attachments Attachments::addAttachmentIf(bool add, const std::function<Attachment()> &attachment) {
        if (add) {
            addAttachment(attachment);
        }
        return *this;
    }

} // vklite
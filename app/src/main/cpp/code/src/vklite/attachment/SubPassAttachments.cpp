//
// Created by leixing on 2025/5/13.
//

#include "SubPassAttachments.h"

namespace vklite {

    SubPassAttachments::SubPassAttachments() = default;

    SubPassAttachments::~SubPassAttachments() = default;

    SubPassAttachments SubPassAttachments::addAttachment(Attachment &&attachment) {
        mAttachments.push_back(attachment);
        return *this;
    }

    SubPassAttachments SubPassAttachments::addAttachment(const std::function<Attachment()> &attachment) {
        mAttachments.push_back(attachment());
        return *this;
    }

    SubPassAttachments SubPassAttachments::addAttachmentIf(bool add, const std::function<Attachment()> &attachment) {
        if (add) {
            addAttachment(attachment);
        }
        return *this;
    }

} // vklite
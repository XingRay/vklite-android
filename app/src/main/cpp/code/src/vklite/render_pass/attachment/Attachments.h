//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <vector>
#include <functional>

#include "Attachment.h"

namespace vklite {

    class Attachments {
    private:
        std::vector<Attachment> mAttachments;

    public:
        Attachments();

        ~Attachments();

        Attachments addAttachment(Attachment &&attachment);

        Attachments addAttachment(const std::function<Attachment()> &attachment);

        Attachments addAttachmentIf(bool add, const std::function<Attachment()> &attachment);
    };

} // vklite

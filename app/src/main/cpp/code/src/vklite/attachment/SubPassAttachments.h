//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <vector>
#include <functional>

#include "vklite/attachment/Attachment.h"

namespace vklite {

    class SubPassAttachments {
    private:
        std::vector<Attachment> mAttachments;

    public:
        SubPassAttachments();

        ~SubPassAttachments();

        SubPassAttachments addAttachment(Attachment attachment);

        SubPassAttachments addAttachment(std::function<Attachment()> attachment);

        SubPassAttachments addAttachmentIf(bool add, std::function<Attachment()> attachment);
    };

} // vklite

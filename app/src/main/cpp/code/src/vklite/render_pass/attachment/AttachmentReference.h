//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "vklite/render_pass/attachment/Attachment.h"

namespace vklite {

    class AttachmentReference {
    private:
        std::reference_wrapper<const Attachment> mAttachment;
        // 启动这个subpass时, 需要将这个附件转化成指定的 Layout
        vk::ImageLayout mLayout;

    public:
        explicit AttachmentReference(const Attachment &attachment, vk::ImageLayout layout);

        ~AttachmentReference();

        [[nodiscard]]
        uint32_t getIndex() const;

        [[nodiscard]]
        vk::AttachmentReference createAttachmentReference() const;
    };

} // vklite

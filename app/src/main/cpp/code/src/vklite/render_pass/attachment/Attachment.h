//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class Subpass;

    class Attachment {
    private:
        uint32_t mIndex;

        vk::Format mFormat;
        vk::SampleCountFlagBits mSampleCount;
        vk::AttachmentLoadOp mLoadOp;
        vk::AttachmentStoreOp mStoreOp;
        vk::AttachmentLoadOp mStencilLoadOp;
        vk::AttachmentStoreOp mStencilStoreOp;
        vk::ImageLayout mInitialLayout;
        vk::ImageLayout mFinalLayout;

    public:
        Attachment();

        ~Attachment();

        [[nodiscard]]
        uint32_t getIndex() const;

        Attachment &format(vk::Format format);

        Attachment &sampleCount(vk::SampleCountFlagBits sampleCount);

        Attachment &loadOp(vk::AttachmentLoadOp loadOp);

        Attachment &storeOp(vk::AttachmentStoreOp storeOp);

        Attachment &stencilLoadOp(vk::AttachmentLoadOp stencilLoadOp);

        Attachment &stencilStoreOp(vk::AttachmentStoreOp stencilStoreOp);

        Attachment &initialLayout(vk::ImageLayout initialLayout);

        Attachment &finalLayout(vk::ImageLayout finalLayout);

        Attachment &index(uint32_t index);

        Attachment &asInputAttachmentUsedIn(Subpass &Subpass, vk::ImageLayout layout);

        Attachment &asInputAttachmentUsedInIf(bool condition, Subpass &Subpass, vk::ImageLayout layout);

        Attachment &asColorAttachmentUsedIn(Subpass &Subpass, vk::ImageLayout layout);

        Attachment &asColorAttachmentUsedInIf(bool condition, Subpass &Subpass, vk::ImageLayout layout);

        Attachment &asResolveAttachmentUsedIn(Subpass &Subpass, vk::ImageLayout layout);

        Attachment &asResolveAttachmentUsedInIf(bool condition, Subpass &Subpass, vk::ImageLayout layout);

        Attachment &asDepthStencilAttachmentUsedIn(Subpass &Subpass, vk::ImageLayout layout);

        Attachment &asDepthStencilAttachmentUsedInIf(bool condition, Subpass &Subpass, vk::ImageLayout layout);

        Attachment &asPreserveAttachmentUsedIn(Subpass &Subpass, vk::ImageLayout layout);

        Attachment &asPreserveAttachmentUsedInIf(bool condition, Subpass &Subpass, vk::ImageLayout layout);

        [[nodiscard]]
        vk::AttachmentDescription createAttachmentDescription() const;

    public:// static

        static Attachment& msaaColorAttachment(Attachment& attachment);

        static Attachment& depthStencilAttachment(Attachment& attachment);

        static Attachment& presentColorAttachment(Attachment& attachment);
    };

} // vklite

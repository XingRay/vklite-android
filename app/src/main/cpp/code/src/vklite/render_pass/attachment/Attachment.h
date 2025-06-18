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
        // 整个 RenderPass 流程启动前附件的 Layout
        vk::ImageLayout mInitialLayout;
        // 整个 RenderPass 流程结束后附件的 Layout
        vk::ImageLayout mFinalLayout;

        // clear value
        vk::ClearValue mClearValue;

    public:
        Attachment();

        ~Attachment();

        [[nodiscard]]
        uint32_t getIndex() const;

        [[nodiscard]]
        vk::ClearValue getClearValue() const;

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

        Attachment &applyIf(bool condition, const std::function<void(Attachment &attachment)> &handler);

        Attachment &clearColorValue(vk::ClearColorValue clearColorValue);

        Attachment &clearColorValue(const std::vector<float> &clearColorValue);

        Attachment &clearColorValue(const std::array<float, 4> &clearColorValue);

        Attachment &clearDepthStencilValue(vk::ClearDepthStencilValue clearDepthStencilValue);

        Attachment &clearDepthValue(float clearDepthValue);

        Attachment &clearStencilValue(uint32_t clearStencilValue);

        [[nodiscard]]
        vk::AttachmentDescription createAttachmentDescription() const;

    public:// static

        static Attachment &msaaColorAttachment(Attachment &attachment);

        static Attachment &depthStencilAttachment(Attachment &attachment);

        static Attachment &presentColorAttachment(Attachment &attachment);
    };

} // vklite

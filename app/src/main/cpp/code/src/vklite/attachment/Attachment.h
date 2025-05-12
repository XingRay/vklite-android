//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <vulkan//vulkan.hpp>

namespace vklite {

    class Attachment {
    private:
        vk::Format mFormat;
        vk::SampleCountFlags mSampleCountFlags;
        vk::AttachmentLoadOp mLoadOp;
        vk::AttachmentStoreOp mStoreOp;
        vk::AttachmentLoadOp mStencilLoadOp;
        vk::AttachmentStoreOp mStencilStoreOp;
        vk::ImageLayout mInitialLayout;
        vk::ImageLayout mFinalLayout;

    public:
        Attachment();

        ~Attachment();

        Attachment &format(vk::Format format);

        Attachment &sampleCountFlags(vk::SampleCountFlags sampleCountFlags);

        Attachment &loadOp(vk::AttachmentLoadOp loadOp);

        Attachment &storeOp(vk::AttachmentStoreOp storeOp);

        Attachment &stencilLoadOp(vk::AttachmentLoadOp stencilLoadOp);

        Attachment &stencilStoreOp(vk::AttachmentStoreOp stencilStoreOp);

        Attachment &initialLayout(vk::ImageLayout initialLayout);

        Attachment &finalLayout(vk::ImageLayout finalLayout);
    };

} // vklite

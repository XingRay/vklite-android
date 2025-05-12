//
// Created by leixing on 2025/5/13.
//

#include "Attachment.h"

namespace vklite {

    Attachment::Attachment() = default;

    Attachment::~Attachment() = default;

    Attachment &Attachment::format(vk::Format format) {
        mFormat = format;
        return *this;
    }

    Attachment &Attachment::sampleCountFlags(vk::SampleCountFlags sampleCountFlags) {
        mSampleCountFlags = sampleCountFlags;
        return *this;
    }

    Attachment &Attachment::loadOp(vk::AttachmentLoadOp loadOp) {
        mLoadOp = loadOp;
        return *this;
    }

    Attachment &Attachment::storeOp(vk::AttachmentStoreOp storeOp) {
        mStoreOp = storeOp;
        return *this;
    }

    Attachment &Attachment::stencilLoadOp(vk::AttachmentLoadOp stencilLoadOp) {
        mStencilLoadOp = stencilLoadOp;
        return *this;
    }

    Attachment &Attachment::stencilStoreOp(vk::AttachmentStoreOp stencilStoreOp) {
        mStencilStoreOp = stencilStoreOp;
        return *this;
    }

    Attachment &Attachment::initialLayout(vk::ImageLayout initialLayout) {
        mInitialLayout = initialLayout;
        return *this;
    }

    Attachment &Attachment::finalLayout(vk::ImageLayout finalLayout) {
        mFinalLayout = finalLayout;
        return *this;
    }

} // vklite
//
// Created by leixing on 2025/5/13.
//

#include "Attachment.h"

namespace vklite {

    Attachment::Attachment()
            : mFormat(vk::Format::eUndefined),
              mSampleCountFlags(vk::SampleCountFlagBits::e1),
              mLoadOp(vk::AttachmentLoadOp::eDontCare),
              mStoreOp(vk::AttachmentStoreOp::eDontCare),
              mStencilLoadOp(vk::AttachmentLoadOp::eDontCare),
              mStencilStoreOp(vk::AttachmentStoreOp::eDontCare),
              mInitialLayout(vk::ImageLayout::eUndefined),
              mFinalLayout(vk::ImageLayout::eUndefined) {};

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

    Attachment Attachment::msaaColorAttachment() {
        Attachment msaaColorAttachment;
        msaaColorAttachment
//                .setFormat(displayFormat)
//                .setSamples(sampleCountFlagBits)
                //载入图像前将帧缓冲清0
                .loadOp(vk::AttachmentLoadOp::eClear)
                        // 渲染图像之后将图像数据保存
                .storeOp(vk::AttachmentStoreOp::eStore)
                        // 模版缓冲, 这里不关注
                .stencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                .stencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                        // 常见的布局
                        //
                        //VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL：用作彩色附件的图像
                        //VK_IMAGE_LAYOUT_PRESENT_SRC_KHR：要在交换链中呈现的图像
                        //VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL：用作内存复制操作目标的图像
                        //
                        // initialLayout 渲染通道开始之前图像将具有的布局
                        // finalLayout 渲染通道完成时自动转换到的布局
                        //
                        // 使用 VK_IMAGE_LAYOUT_UNDEFINED 意味着我们不关心图像以前的布局
                        // 这个特殊值的警告是图像的内容不能保证被保留，但这并不重要，因为我们无论如何要清除
                .initialLayout(vk::ImageLayout::eUndefined)
                .finalLayout(vk::ImageLayout::eColorAttachmentOptimal);
        return msaaColorAttachment;
    }

    Attachment Attachment::depthStencilAttachment() {
        Attachment depthStencilAttachment;
        depthStencilAttachment
//                .setFormat(device.getPhysicalDevice().findDepthFormat())
//                .setSamples(sampleCountFlagBits)
                .loadOp(vk::AttachmentLoadOp::eClear)
                .storeOp(vk::AttachmentStoreOp::eDontCare)
                .stencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                .stencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                .initialLayout(vk::ImageLayout::eUndefined)
                .finalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
        return depthStencilAttachment;
    }

    Attachment Attachment::presentColorAttachment() {
        Attachment presentColorAttachment;
        presentColorAttachment
//                .setFormat(displayFormat)
                .sampleCountFlags(vk::SampleCountFlagBits::e1)
                .loadOp(vk::AttachmentLoadOp::eDontCare)
                .storeOp(vk::AttachmentStoreOp::eStore)
                .stencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                .stencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                .initialLayout(vk::ImageLayout::eUndefined)
                .finalLayout(vk::ImageLayout::ePresentSrcKHR);
        return presentColorAttachment;
    }

} // vklite
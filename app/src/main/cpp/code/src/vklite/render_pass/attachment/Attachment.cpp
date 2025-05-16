//
// Created by leixing on 2025/5/13.
//

#include "Attachment.h"
#include "vklite/render_pass/subpass/Subpass.h"

namespace vklite {

    Attachment::Attachment()
            : mIndex(0),
              mFormat(vk::Format::eUndefined),
              mSampleCount(vk::SampleCountFlagBits::e1),
              mLoadOp(vk::AttachmentLoadOp::eDontCare),
              mStoreOp(vk::AttachmentStoreOp::eDontCare),
              mStencilLoadOp(vk::AttachmentLoadOp::eDontCare),
              mStencilStoreOp(vk::AttachmentStoreOp::eDontCare),
              mInitialLayout(vk::ImageLayout::eUndefined),
              mFinalLayout(vk::ImageLayout::eUndefined),
              mClearValue(vk::ClearValue{}) {};

    Attachment::~Attachment() = default;

    uint32_t Attachment::getIndex() const {
        return mIndex;
    }

    vk::ClearValue Attachment::getClearValue() const {
        return mClearValue;
    }

    Attachment &Attachment::format(vk::Format format) {
        mFormat = format;
        return *this;
    }

    Attachment &Attachment::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mSampleCount = sampleCount;
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

    Attachment &Attachment::index(uint32_t index) {
        mIndex = index;
        return *this;
    }

    Attachment &Attachment::asInputAttachmentUsedIn(Subpass &Subpass, vk::ImageLayout layout) {
        Subpass.addInputAttachment(*this, layout);
        return *this;
    }

    Attachment &Attachment::asInputAttachmentUsedInIf(bool condition, Subpass &Subpass, vk::ImageLayout layout) {
        if (condition) {
            asInputAttachmentUsedIn(Subpass, layout);
        }
        return *this;
    }

    Attachment &Attachment::asColorAttachmentUsedIn(Subpass &Subpass, vk::ImageLayout layout) {
        Subpass.addColorAttachment(*this, layout);
        return *this;
    }

    Attachment &Attachment::asColorAttachmentUsedInIf(bool condition, Subpass &Subpass, vk::ImageLayout layout) {
        if (condition) {
            asColorAttachmentUsedIn(Subpass, layout);
        }
        return *this;
    }

    Attachment &Attachment::asResolveAttachmentUsedIn(Subpass &Subpass, vk::ImageLayout layout) {
        Subpass.addResolveAttachment(*this, layout);
        return *this;
    }

    Attachment &Attachment::asResolveAttachmentUsedInIf(bool condition, Subpass &Subpass, vk::ImageLayout layout) {
        if (condition) {
            asResolveAttachmentUsedIn(Subpass, layout);
        }
        return *this;
    }

    Attachment &Attachment::asDepthStencilAttachmentUsedIn(Subpass &Subpass, vk::ImageLayout layout) {
        Subpass.setDepthStencilAttachment(*this, layout);
        return *this;
    }

    Attachment &Attachment::asDepthStencilAttachmentUsedInIf(bool condition, Subpass &Subpass, vk::ImageLayout layout) {
        if (condition) {
            asDepthStencilAttachmentUsedIn(Subpass, layout);
        }
        return *this;
    }

    Attachment &Attachment::asPreserveAttachmentUsedIn(Subpass &Subpass, vk::ImageLayout layout) {
        Subpass.addPreserveAttachment(*this, layout);
        return *this;
    }

    Attachment &Attachment::asPreserveAttachmentUsedInIf(bool condition, Subpass &Subpass, vk::ImageLayout layout) {
        if (condition) {
            asPreserveAttachmentUsedIn(Subpass, layout);
        }
        return *this;
    }

    Attachment &Attachment::applyIf(bool condition, const std::function<void(Attachment &attachment)> &handler) {
        if (condition) {
            handler(*this);
        }
        return *this;
    }

    Attachment &Attachment::clearColorValue(vk::ClearColorValue value) {
        mClearValue = vk::ClearValue(value);
        return *this;
    }

    Attachment &Attachment::clearColorValue(const std::array<float, 4> &value) {
        clearColorValue(vk::ClearColorValue(value));
        return *this;
    }

    Attachment &Attachment::clearColorValue(const std::vector<float> &value) {
        clearColorValue(std::array<float, 4>{value[0], value[1], value[2], value[3]});
        return *this;
    }

    Attachment &Attachment::clearDepthStencilValue(vk::ClearDepthStencilValue value) {
        mClearValue = vk::ClearValue(value);
        return *this;
    }

    Attachment &Attachment::clearDepthValue(float value) {
        vk::ClearDepthStencilValue depthValue;
        depthValue.setDepth(value);
        clearDepthStencilValue(depthValue);
        return *this;
    }

    Attachment &Attachment::clearStencilValue(uint32_t value) {
        vk::ClearDepthStencilValue depthValue;
        depthValue.setStencil(value);
        clearDepthStencilValue(depthValue);
        return *this;
    }

    vk::AttachmentDescription Attachment::createAttachmentDescription() const {
        vk::AttachmentDescription attachmentDescription{};

        attachmentDescription
                .setFormat(mFormat)
                .setSamples(mSampleCount)
                .setLoadOp(mLoadOp)
                .setStoreOp(mStoreOp)
                .setStencilLoadOp(mStencilLoadOp)
                .setStencilStoreOp(mStencilStoreOp)
                .setInitialLayout(mInitialLayout)
                .setFinalLayout(mFinalLayout);

        return attachmentDescription;
    }

    // static

    Attachment &Attachment::msaaColorAttachment(Attachment &attachment) {
        attachment
//                .setFormat(displayFormat)
//                .setSamples(sampleCountFlagBits)
                //载入图像前将帧缓冲设置为 ClearValue, 需要配置 ClearValues
                .loadOp(vk::AttachmentLoadOp::eClear)
                        // 渲染图像之后将图像数据保存
                .storeOp(vk::AttachmentStoreOp::eDontCare)
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
        return attachment;
    }

    Attachment &Attachment::depthStencilAttachment(Attachment &attachment) {
        attachment
//                .setFormat(device.getPhysicalDevice().findDepthFormat())
//                .setSamples(sampleCountFlagBits)
                .loadOp(vk::AttachmentLoadOp::eClear)
                .storeOp(vk::AttachmentStoreOp::eDontCare)
                .stencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                .stencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                .initialLayout(vk::ImageLayout::eUndefined)
                .finalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
        return attachment;
    }

    Attachment &Attachment::presentColorAttachment(Attachment &attachment) {
        attachment
//                .setFormat(displayFormat)
                .sampleCount(vk::SampleCountFlagBits::e1)
                .loadOp(vk::AttachmentLoadOp::eClear)
                .storeOp(vk::AttachmentStoreOp::eDontCare)
                .stencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                .stencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                .initialLayout(vk::ImageLayout::eUndefined)
                .finalLayout(vk::ImageLayout::ePresentSrcKHR);
        return attachment;
    }

} // vklite
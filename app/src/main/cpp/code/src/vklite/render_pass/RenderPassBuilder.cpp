//
// Created by leixing on 2025/5/1.
//

#include "RenderPassBuilder.h"

namespace vklite {

    RenderPassBuilder::RenderPassBuilder() = default;

    RenderPassBuilder::~RenderPassBuilder() = default;

    RenderPassBuilder &RenderPassBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::flags(vk::RenderPassCreateFlags flags) {
        mRenderPassCreateInfo.setFlags(flags);
        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::addSubpass(const std::function<void(Subpass &subpass, const std::vector<Subpass> &subpasses)> &configure) {
        if (mAddAttachmentInvoked) {
            throw std::runtime_error("addSubpass must invoke before all of addAttachment()");
        }
        Subpass Subpass{};
        Subpass.index(mSubpasses.size());
        configure(Subpass, mSubpasses);
        mSubpasses.push_back(std::move(Subpass));
        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::addAttachment(const std::function<void(Attachment &attachment, std::vector<Subpass> &subpasses)> &configure) {
        // ensure subpass added before addAttachment()
        mAddAttachmentInvoked = true;

        mAttachments.emplace_back();
        Attachment &attachment = mAttachments.back();
        attachment.index(mAttachments.size() - 1);

        configure(attachment, mSubpasses);

        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::addAttachmentIf(bool condition, const std::function<void(Attachment &attachment, std::vector<Subpass> &subpasses)> &configure) {
        if (condition) {
            addAttachment(configure);
        }
        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::renderArea(vk::Rect2D renderArea) {
        mRenderPassBeginInfo.getBeginInfo().renderArea = renderArea;
        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::renderArea(int32_t x, int32_t y, uint32_t width, uint32_t height) {
        renderArea(vk::Rect2D{vk::Offset2D{x, y}, vk::Extent2D{width, height}});
        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::renderAreaOffset(vk::Offset2D offset) {
        mRenderPassBeginInfo.getBeginInfo().renderArea.setOffset(offset);
        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::renderAreaOffset(int32_t x, int32_t y) {
        renderAreaOffset(vk::Offset2D{x, y});
        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::renderAreaExtend(vk::Extent2D extent) {
        mRenderPassBeginInfo.getBeginInfo().renderArea.setExtent(extent);
        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::renderAreaExtend(uint32_t width, uint32_t height) {
        renderAreaExtend(vk::Extent2D{width, height});
        return *this;
    }

    RenderPassBuilder &RenderPassBuilder::subpassContents(vk::SubpassContents subpassContents) {
        mRenderPassBeginInfo.subpassContents(subpassContents);
        return *this;
    }

    RenderPass RenderPassBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("mDevice== nullptr , must invoke RenderPassBuilder::device(vk::Device device) ");
        }

        std::vector<vk::AttachmentDescription> attachmentDescriptions;
        attachmentDescriptions.reserve(mAttachments.size());
        for (const Attachment &attachment: mAttachments) {
            attachmentDescriptions.push_back(attachment.createAttachmentDescription());
        }
        mRenderPassCreateInfo.setAttachments(attachmentDescriptions);


        std::vector<SubpassDescription> subpassDescriptions;
        subpassDescriptions.reserve(mSubpasses.size());
        for (const Subpass &subpass: mSubpasses) {
            subpassDescriptions.push_back(subpass.createSubpassDescription());
        }

        std::vector<vk::SubpassDescription> vkSubpassDescriptions;
        vkSubpassDescriptions.reserve(subpassDescriptions.size());
        for (const SubpassDescription &subpassDescription: subpassDescriptions) {
            vkSubpassDescriptions.push_back(subpassDescription.createSubpassDescription());
        }
        mRenderPassCreateInfo.setSubpasses(vkSubpassDescriptions);


        std::vector<vk::SubpassDependency> subpassDependencies;
        for (const Subpass &subpass: mSubpasses) {
            std::vector<vk::SubpassDependency> dependencies = subpass.createSubpassDependencies();
            subpassDependencies.insert(subpassDependencies.end(),
                                       std::make_move_iterator(dependencies.begin()),
                                       std::make_move_iterator(dependencies.end())
            );
        }
        mRenderPassCreateInfo.setDependencies(subpassDependencies);


        std::vector<vk::ClearValue> clearValues;
        clearValues.reserve(mAttachments.size());
        for (const Attachment &attachment: mAttachments) {
            clearValues.push_back(attachment.getClearValue());
        }
        mRenderPassBeginInfo.clearValues(std::move(clearValues));


        vk::RenderPass renderPass = mDevice.createRenderPass(mRenderPassCreateInfo);

//        return RenderPass(mDevice, renderPass, std::move(mRenderPassBeginInfo));
        return {mDevice, renderPass, std::move(mRenderPassBeginInfo)};
    }

    std::unique_ptr<RenderPass> RenderPassBuilder::buildUnique() {
        return std::make_unique<RenderPass>(build());
    }

} // vklite
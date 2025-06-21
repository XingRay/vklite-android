//
// Created by leixing on 2025/5/1.
//

#include "FramebufferBuilder.h"

namespace vklite {

    FramebufferBuilder::FramebufferBuilder() {
        mFramebufferCreateInfo.layers = 1;
    }

    FramebufferBuilder::~FramebufferBuilder() = default;

    FramebufferBuilder &FramebufferBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    FramebufferBuilder &FramebufferBuilder::renderPass(vk::RenderPass renderPass) {
        mFramebufferCreateInfo.setRenderPass(renderPass);
        return *this;
    }

    FramebufferBuilder &FramebufferBuilder::width(uint32_t width) {
        mFramebufferCreateInfo.setWidth(width);
        return *this;
    }

    FramebufferBuilder &FramebufferBuilder::height(uint32_t height) {
        mFramebufferCreateInfo.setHeight(height);
        return *this;
    }

    FramebufferBuilder &FramebufferBuilder::size(vk::Extent2D size) {
        width(size.width);
        height(size.height);
        return *this;
    }

    FramebufferBuilder &FramebufferBuilder::layers(uint32_t layers) {
        mFramebufferCreateInfo.setLayers(layers);
        return *this;
    }

    FramebufferBuilder &FramebufferBuilder::addAttachment(vk::ImageView attachment) {
        mAttachments.push_back(attachment);
        return *this;
    }

    FramebufferBuilder &FramebufferBuilder::addAttachmentIf(bool condition, vk::ImageView attachment) {
        if (condition) {
            mAttachments.push_back(attachment);
        }
        return *this;
    }

    FramebufferBuilder &FramebufferBuilder::addAttachmentIf(bool condition, const std::function<vk::ImageView()> &attachment) {
        if (condition) {
            mAttachments.push_back(attachment());
        }
        return *this;
    }

    Framebuffer FramebufferBuilder::build() {
        // check params
        if (mDevice == nullptr) {
            throw std::runtime_error("mDevice== nullptr, must invoke FramebufferBuilder::device(vk::Device device)");
        }
        if (mFramebufferCreateInfo.renderPass == nullptr) {
            throw std::runtime_error("mFramebufferCreateInfo.renderPass== nullptr, must invoke FramebufferBuilder::renderPass(vk::RenderPass renderPass)");
        }

        // create vk::Framebuffer
        mFramebufferCreateInfo.setAttachments(mAttachments);
        vk::Framebuffer mFramebuffer = mDevice.createFramebuffer(mFramebufferCreateInfo);

        return Framebuffer(mDevice, mFramebuffer);
    }

    std::unique_ptr<Framebuffer> FramebufferBuilder::buildUnique() {
        return std::make_unique<Framebuffer>(build());
    }

} // vklite
//
// Created by leixing on 2025/5/1.
//

#include "FrameBufferBuilder.h"

namespace vklite {

    FrameBufferBuilder::FrameBufferBuilder() {
        mFramebufferCreateInfo.layers = 1;
    }

    FrameBufferBuilder::~FrameBufferBuilder() = default;

    FrameBufferBuilder &FrameBufferBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::renderPass(vk::RenderPass renderPass) {
        mFramebufferCreateInfo.setRenderPass(renderPass);
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::width(uint32_t width) {
        mFramebufferCreateInfo.setWidth(width);
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::height(uint32_t height) {
        mFramebufferCreateInfo.setHeight(height);
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::layers(uint32_t layers) {
        mFramebufferCreateInfo.setLayers(layers);
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::addAttachment(vk::ImageView attachment) {
        mAttachments.push_back(attachment);
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::addAttachmentIf(bool condition, vk::ImageView attachment) {
        if (condition) {
            mAttachments.push_back(attachment);
        }
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::addAttachmentIf(bool condition, const std::function<vk::ImageView()> &attachment) {
        if (condition) {
            mAttachments.push_back(attachment());
        }
        return *this;
    }

    FrameBuffer FrameBufferBuilder::build() {
        // check params
        if(mDevice== nullptr){
            throw std::runtime_error("mDevice== nullptr, must invoke FrameBufferBuilder::device(vk::Device device)");
        }
        if(mFramebufferCreateInfo.renderPass== nullptr){
            throw std::runtime_error("mFramebufferCreateInfo.renderPass== nullptr, must invoke FrameBufferBuilder::renderPass(vk::RenderPass renderPass)");
        }

        // create vk::Framebuffer
        mFramebufferCreateInfo.setAttachments(mAttachments);
        vk::Framebuffer mFrameBuffer = mDevice.createFramebuffer(mFramebufferCreateInfo);

//        return FrameBuffer(mDevice, mFrameBuffer);
        return {mDevice, mFrameBuffer};
    }

    std::unique_ptr<FrameBuffer> FrameBufferBuilder::buildUnique() {
        return std::make_unique<FrameBuffer>(build());
    }

} // vklite
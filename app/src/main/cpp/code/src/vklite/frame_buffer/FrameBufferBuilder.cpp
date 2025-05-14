//
// Created by leixing on 2025/5/1.
//

#include "FrameBufferBuilder.h"

namespace vklite {

    FrameBufferBuilder::FrameBufferBuilder()
            : mLayers(1), mWidth(0), mHeight(0) {}

    FrameBufferBuilder::~FrameBufferBuilder() = default;

    FrameBufferBuilder &FrameBufferBuilder::width(uint32_t width) {
        mWidth = width;
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::height(uint32_t height) {
        mHeight = height;
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::layers(uint32_t layers) {
        mLayers = layers;
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::addAttachment(vk::ImageView attachment) {
        mAttachments.push_back(attachment);
        return *this;
    }

    FrameBufferBuilder &FrameBufferBuilder::addAttachmentIf(bool condition, vk::ImageView attachment) {
        if(condition){
            mAttachments.push_back(attachment);
        }
        return *this;
    }

    FrameBuffer FrameBufferBuilder::build(const vklite::Device &device, const vklite::RenderPass &renderPass) {
//        return FrameBuffer(device, renderPass, mAttachments, mWidth, mHeight, mLayers);
        return {device, renderPass, mAttachments, mWidth, mHeight, mLayers};
    }

    std::unique_ptr<FrameBuffer> FrameBufferBuilder::buildUnique(const vklite::Device &device, const vklite::RenderPass &renderPass) {
        return std::make_unique<FrameBuffer>(device, renderPass, mAttachments, mWidth, mHeight, mLayers);
    }

} // vklite
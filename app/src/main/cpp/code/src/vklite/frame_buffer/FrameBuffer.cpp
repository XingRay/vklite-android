//
// Created by leixing on 2024/12/31.
//

#include "FrameBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    FrameBuffer::FrameBuffer(const Device &device, const RenderPass &renderPass, const std::vector<vk::ImageView> &attachments, uint32_t width, uint32_t height, uint32_t layers)
            : mDevice(device) {
        LOG_D("FrameBuffer::FrameBuffer");

        vk::FramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo
                .setRenderPass(renderPass.getRenderPass())
                .setAttachments(attachments)
                .setWidth(width)
                .setHeight(height)
                .setLayers(layers);

        mFrameBuffer = device.getDevice().createFramebuffer(framebufferCreateInfo);
        LOG_D("device.getDevice().createFramebuffer() => mFrameBuffer: %p", static_cast<void *>(mFrameBuffer));
    }

//    FrameBuffer::FrameBuffer(FrameBuffer &&other) noexcept
//            : mDevice(other.mDevice), mFrameBuffer(other.mFrameBuffer) {
//        other.mFrameBuffer = nullptr;
//    }

    FrameBuffer::FrameBuffer(FrameBuffer &&other) noexcept
            : mDevice(other.mDevice),
              mFrameBuffer(std::exchange(other.mFrameBuffer, nullptr)) {
        LOG_D("FrameBuffer::FrameBuffer(FrameBuffer &&other)");
    }

    FrameBuffer::~FrameBuffer() {
        LOG_D("FrameBuffer::~FrameBuffer");
        if (mFrameBuffer != nullptr) {
            LOG_D("vkDevice.destroy(mFrameBuffer); mFrameBuffer: %p", static_cast<void *>(mFrameBuffer));
            vk::Device vkDevice = mDevice.getDevice();
            vkDevice.destroy(mFrameBuffer);
        } else {
            LOG_D("mFrameBuffer is nullptr, 'vkDevice.destroy(mFrameBuffer);' skipped");
        }
    }

    const vk::Framebuffer &FrameBuffer::getFrameBuffer() const {
        return mFrameBuffer;
    }
} // vklite
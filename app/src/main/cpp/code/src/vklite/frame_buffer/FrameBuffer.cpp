//
// Created by leixing on 2024/12/31.
//

#include "FrameBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    FrameBuffer::FrameBuffer(const Device &device, const RenderPass &renderPass, const std::vector<vk::ImageView> &attachments, uint32_t width, uint32_t height, uint32_t layers)
            : mDevice(device) {

        vk::FramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo
                .setRenderPass(renderPass.getRenderPass())
                .setAttachments(attachments)
                .setWidth(width)
                .setHeight(height)
                .setLayers(layers);

        mFrameBuffer = device.getDevice().createFramebuffer(framebufferCreateInfo);
    }

    FrameBuffer::FrameBuffer(FrameBuffer &&other) noexcept
            : mDevice(other.mDevice),
              mFrameBuffer(std::exchange(other.mFrameBuffer, nullptr)) {}

    FrameBuffer::~FrameBuffer() {
        if (mFrameBuffer != nullptr) {
            const vk::Device &vkDevice = mDevice.getDevice();
            vkDevice.destroy(mFrameBuffer);
        }
    }

    const vk::Framebuffer &FrameBuffer::getFrameBuffer() const {
        return mFrameBuffer;
    }

} // vklite
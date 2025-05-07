//
// Created by leixing on 2024/12/31.
//

#include "FrameBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    FrameBuffer::FrameBuffer(const Device &device, const RenderPass &renderPass, const CommandPool &commandPool,
                             std::vector<ImageView> &&displayImageViews,
                             vk::Format displayFormat,
                             vk::Extent2D displaySize,
                             vk::SampleCountFlagBits sampleCountFlagBits)
            : mDevice(device), mDisplayImageViews(std::move(displayImageViews)) {
        LOG_D("FrameBuffer::FrameBuffer");
        std::tie(mColorImage, mColorDeviceMemory) = device.createImage(displaySize.width, displaySize.height, 1,
                                                                       sampleCountFlagBits,
                                                                       displayFormat,
                                                                       vk::ImageTiling::eOptimal,
                                                                       vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
                                                                       vk::MemoryPropertyFlagBits::eDeviceLocal);
        mColorImageView = device.createImageView(mColorImage, displayFormat, vk::ImageAspectFlagBits::eColor, 1);

        vk::Format depthFormat = device.getPhysicalDevice().findDepthFormat();

        std::tie(mDepthImage, mDepthDeviceMemory) = device.createImage(displaySize.width, displaySize.height, 1,
                                                                       sampleCountFlagBits,
                                                                       depthFormat,
                                                                       vk::ImageTiling::eOptimal,
                                                                       vk::ImageUsageFlagBits::eDepthStencilAttachment,
                                                                       vk::MemoryPropertyFlagBits::eDeviceLocal);
        mDepthImageView = device.createImageView(mDepthImage, depthFormat, vk::ImageAspectFlagBits::eDepth, 1);

        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) -> void {
            VulkanUtil::recordTransitionImageLayoutCommand(commandBuffer, mDepthImage, depthFormat, vk::ImageLayout::eUndefined,
                                                           vk::ImageLayout::eDepthStencilAttachmentOptimal, 1,
                                                           vk::QueueFamilyIgnored, vk::QueueFamilyIgnored);
        });

        mFrameBuffers.resize(mDisplayImageViews.size());

        for (int i = 0; i < mDisplayImageViews.size(); i++) {
            std::array<vk::ImageView, 3> attachments = {
                    mColorImageView,
                    mDepthImageView,
                    mDisplayImageViews[i].getImageView(),
            };

            vk::FramebufferCreateInfo framebufferCreateInfo{};
            framebufferCreateInfo
                    .setRenderPass(renderPass.getRenderPass())
                    .setAttachments(attachments)
                    .setWidth(displaySize.width)
                    .setHeight(displaySize.height)
                    .setLayers(1);

            mFrameBuffers[i] = device.getDevice().createFramebuffer(framebufferCreateInfo);
        }
    }

    FrameBuffer::~FrameBuffer() {
        LOG_D("FrameBuffer::~FrameBuffer");
        vk::Device device = mDevice.getDevice();

        for (const auto &frameBuffer: mFrameBuffers) {
            device.destroy(frameBuffer);
        }

        device.destroy(mDepthImageView);
        device.destroy(mDepthImage);
        device.freeMemory(mDepthDeviceMemory);

        device.destroy(mColorImageView);
        device.destroy(mColorImage);
        device.freeMemory(mColorDeviceMemory);
    }

    const std::vector<vk::Framebuffer> &FrameBuffer::getFrameBuffers() const {
        return mFrameBuffers;
    }
} // vklite
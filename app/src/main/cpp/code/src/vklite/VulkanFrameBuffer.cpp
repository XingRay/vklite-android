//
// Created by leixing on 2024/12/31.
//

#include "VulkanFrameBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    VulkanFrameBuffer::VulkanFrameBuffer(const Device &vulkanDevice, const Swapchain &vulkanSwapchain, const VulkanRenderPass &vulkanRenderPass, const VulkanCommandPool &commandPool)
            : mDevice(vulkanDevice) {

        const vk::Device device = vulkanDevice.getDevice();
        vk::Format colorFormat = vulkanSwapchain.getDisplayFormat();
        vk::Extent2D displaySize = vulkanSwapchain.getDisplaySize();

        std::tie(mColorImage, mColorDeviceMemory) = vulkanDevice.createImage(displaySize.width, displaySize.height, 1,
                //todo: msaa samples
                /*vulkanDevice.getMsaaSamples(),*/vk::SampleCountFlagBits::e1,
                                                                             colorFormat,
                                                                             vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
                                                                             vk::MemoryPropertyFlagBits::eDeviceLocal);
        mColorImageView = vulkanDevice.createImageView(mColorImage, colorFormat, vk::ImageAspectFlagBits::eColor, 1);

        vk::Format depthFormat = vulkanDevice.getPhysicalDevice().findDepthFormat();

        std::tie(mDepthImage, mDepthDeviceMemory) = vulkanDevice.createImage(displaySize.width, displaySize.height, 1,
                //todo: msaa samples
                /*vulkanDevice.getMsaaSamples(),*/vk::SampleCountFlagBits::e1,
                                                                             depthFormat,
                                                                             vk::ImageTiling::eOptimal,
                                                                             vk::ImageUsageFlagBits::eDepthStencilAttachment,
                                                                             vk::MemoryPropertyFlagBits::eDeviceLocal);
        mDepthImageView = vulkanDevice.createImageView(mDepthImage, depthFormat, vk::ImageAspectFlagBits::eDepth, 1);

        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) -> void {
            VulkanUtil::recordTransitionImageLayoutCommand(commandBuffer, mDepthImage, depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1,
                                                           vk::QueueFamilyIgnored, vk::QueueFamilyIgnored);
        });

        mFrameBuffers.resize(vulkanSwapchain.getImageCount());
        std::vector<vk::ImageView> imageViews = vulkanSwapchain.getDisplayImageViews();

        for (int i = 0; i < vulkanSwapchain.getImageCount(); i++) {
            std::array<vk::ImageView, 3> attachments = {
                    mColorImageView,
                    mDepthImageView,
                    imageViews[i],
            };

            vk::FramebufferCreateInfo framebufferCreateInfo{};
            framebufferCreateInfo.setRenderPass(vulkanRenderPass.getRenderPass())
                    .setAttachments(attachments)
                    .setWidth(displaySize.width)
                    .setHeight(displaySize.height)
                    .setLayers(1);

            mFrameBuffers[i] = device.createFramebuffer(framebufferCreateInfo);
        }
    }

    VulkanFrameBuffer::~VulkanFrameBuffer() {
        LOG_D("VulkanFrameBuffer::~VulkanFrameBuffer()");
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

    const std::vector<vk::Framebuffer> &VulkanFrameBuffer::getFrameBuffers() const {
        return mFrameBuffers;
    }
} // vklite
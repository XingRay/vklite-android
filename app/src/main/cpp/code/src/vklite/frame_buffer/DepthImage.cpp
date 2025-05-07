//
// Created by leixing on 2025/5/7.
//

#include "DepthImage.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    DepthImage::DepthImage(const Device &device, uint32_t width, uint32_t height, vk::SampleCountFlagBits sampleCountFlagBits, vk::Format depthFormat)
            : mDevice(device), mDepthFormat(depthFormat) {
//        vk::Format depthFormat = device.getPhysicalDevice().findDepthFormat();

        std::tie(mDepthImage, mDepthDeviceMemory) = device.createImage(width, height, 1,
                                                                       sampleCountFlagBits,
                                                                       depthFormat,
                                                                       vk::ImageTiling::eOptimal,
                                                                       vk::ImageUsageFlagBits::eDepthStencilAttachment,
                                                                       vk::MemoryPropertyFlagBits::eDeviceLocal);
        mDepthImageView = device.createImageView(mDepthImage, depthFormat, vk::ImageAspectFlagBits::eDepth, 1);
    }

    DepthImage::~DepthImage() {
        const vk::Device &vkDevice = mDevice.getDevice();
        vkDevice.destroy(mDepthImageView);
        vkDevice.destroy(mDepthImage);
        vkDevice.freeMemory(mDepthDeviceMemory);
    }

    void DepthImage::recordCommandTransitionImageLayout(const vk::CommandBuffer &commandBuffer) {
        VulkanUtil::recordTransitionImageLayoutCommand(commandBuffer, mDepthImage, mDepthFormat, vk::ImageLayout::eUndefined,
                                                       vk::ImageLayout::eDepthStencilAttachmentOptimal, 1,
                                                       vk::QueueFamilyIgnored, vk::QueueFamilyIgnored);
    }

    void DepthImage::transitionImageLayout(const CommandPool &commandPool) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) -> void {
            recordCommandTransitionImageLayout(commandBuffer);
        });
    }

} // vklite
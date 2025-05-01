//
// Created by leixing on 2024/12/31.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/swapchain/Swapchain.h"
#include "VulkanRenderPass.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class VulkanFrameBuffer {
    private:
        const Device &mDevice;

        vk::Image mColorImage;
        vk::DeviceMemory mColorDeviceMemory;
        vk::ImageView mColorImageView;

        vk::Image mDepthImage;
        vk::DeviceMemory mDepthDeviceMemory;
        vk::ImageView mDepthImageView;

        std::vector<vk::Framebuffer> mFrameBuffers;

    public:
        VulkanFrameBuffer(const Device &device, const Swapchain &vulkanSwapchain, const VulkanRenderPass &vulkanRenderPass, const CommandPool &commandPool);

        ~VulkanFrameBuffer();

        [[nodiscard]]
        const std::vector<vk::Framebuffer> &getFrameBuffers() const;

    private:

    };

} // vklite

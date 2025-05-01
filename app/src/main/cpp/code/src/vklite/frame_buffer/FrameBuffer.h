//
// Created by leixing on 2024/12/31.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/swapchain/Swapchain.h"
#include "vklite/render_pass/RenderPass.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class FrameBuffer {
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
        FrameBuffer(const Device &device, const RenderPass &renderPass, const CommandPool &commandPool,
                    const std::vector<vk::ImageView> &displayImageViews,
                    vk::Format displayFormat,
                    vk::Extent2D displaySize,
                    vk::SampleCountFlagBits sampleCountFlagBits);

        ~FrameBuffer();

        [[nodiscard]]
        const std::vector<vk::Framebuffer> &getFrameBuffers() const;

    private:

    };

} // vklite

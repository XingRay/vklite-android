//
// Created by leixing on 2025/5/7.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "vklite/device/Device.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class DepthImage {
    private:
        const Device &mDevice;
        vk::Format mDepthFormat;

        vk::Image mDepthImage;
        vk::DeviceMemory mDepthDeviceMemory;
        vk::ImageView mDepthImageView;

    public:
        DepthImage(const Device &device, uint32_t width, uint32_t height, vk::SampleCountFlagBits sampleCountFlagBits, vk::Format depthFormat);

        ~DepthImage();

        void recordCommandTransitionImageLayout(const vk::CommandBuffer &commandBuffer);

        void transitionImageLayout(const CommandPool &commandPool);

    };

} // vklite

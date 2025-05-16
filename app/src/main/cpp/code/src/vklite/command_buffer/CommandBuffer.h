//
// Created by leixing on 2025/5/16.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include <vklite/device/Device.h>

namespace vklite {

    class CommandPool;

    class CommandBuffer {
    private:
        const Device &mDevice;
        const CommandPool &mCommandPool;
        vk::CommandBuffer mCommandBuffer;

    public:
        CommandBuffer(const Device &device, const CommandPool &commandPool, const vk::CommandBuffer &commandBuffer);

        ~CommandBuffer();

        [[nodiscard]]
        const vk::CommandBuffer &getCommandBuffer() const;
    };

} // vklite

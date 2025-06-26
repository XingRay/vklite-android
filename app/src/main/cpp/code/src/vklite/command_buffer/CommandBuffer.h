//
// Created by leixing on 2025/5/30.
//

#pragma once

#include "CommandBufferInterface.h"

namespace vklite {

    class CommandBuffer : public CommandBufferInterface {
    private:
        vk::Device mDevice;
        vk::CommandPool mCommandPool;
        vk::CommandBuffer mCommandBuffer;

    public:
        explicit CommandBuffer(vk::Device device, vk::CommandPool commandPool, vk::CommandBuffer commandBuffer);

        ~CommandBuffer() override;

        CommandBuffer(const CommandBuffer &other) = delete;

        CommandBuffer &operator=(const CommandBuffer &other) = delete;

        CommandBuffer(CommandBuffer &&other) noexcept;

        CommandBuffer &operator=(CommandBuffer &&other) noexcept;

        [[nodiscard]]
        const vk::CommandBuffer &getVkCommandBuffer() const override;

    };

} // vklite

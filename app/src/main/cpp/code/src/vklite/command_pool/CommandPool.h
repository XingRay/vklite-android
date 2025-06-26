//
// Created by leixing on 2024/12/30.
//

#pragma once

#include <functional>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/command_pool/CommandPoolMeta.h"
#include "vklite/command_buffer/CommandBuffers.h"
#include "vklite/command_buffer/CommandBuffer.h"
#include "vklite/device/Queue.h"

namespace vklite {

    class CommandPool {
    private:
        vk::Device mDevice;
        vk::CommandPool mCommandPool;

        CommandPoolMeta mMeta;

    public:
        explicit CommandPool(vk::Device device, vk::CommandPool commandPool, CommandPoolMeta meta);

        ~CommandPool();

        CommandPool(const CommandPool &other) = delete;

        CommandPool &operator=(const CommandPool &other) = delete;

        CommandPool(CommandPool &&other) noexcept;

        CommandPool &operator=(CommandPool &&other) noexcept;

        [[nodiscard]]
        const vk::CommandPool &getVkCommandPool() const;

        [[nodiscard]]
        CommandBuffers allocate(uint32_t count, vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary) const;

        [[nodiscard]]
        std::unique_ptr<CommandBuffers> allocateUnique(uint32_t count, vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary) const;

        [[nodiscard]]
        CommandBuffer allocateOne(vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary) const;

        [[nodiscard]]
        std::unique_ptr<CommandBuffer> allocateOneUnique(vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary) const;

        void submit(const vk::Queue &queue, const std::function<void(const vk::CommandBuffer &)> &command) const;

        void submit(const std::function<void(const vk::CommandBuffer &)> &command) const;

        void submit(const Queue &queue, const std::function<void(const vk::CommandBuffer &)> &command) const;
    };

} // vklite

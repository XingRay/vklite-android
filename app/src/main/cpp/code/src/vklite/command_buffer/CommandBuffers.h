//
// Created by leixing on 2025/5/15.
//

#pragma once

#include <vector>
#include <cstddef>

#include <vulkan/vulkan.hpp>

#include "vklite/command_buffer/PooledCommandBuffer.h"

namespace vklite {

    class CommandBuffers {
    private:
        vk::Device mDevice;
        vk::CommandPool mCommandPool;
        std::vector<vk::CommandBuffer> mCommandBuffers;
        std::vector<PooledCommandBuffer> mPooledCommandBuffers;

    public:
        explicit CommandBuffers(vk::Device device,
                                vk::CommandPool commandPool,
                                std::vector<vk::CommandBuffer> &&commandBuffers);

        ~CommandBuffers();

        CommandBuffers(const CommandBuffers &other) = delete;

        CommandBuffers &operator=(const CommandBuffers &other) = delete;

        CommandBuffers(CommandBuffers &&other) noexcept;

        CommandBuffers &operator=(CommandBuffers &&other) noexcept;

        const PooledCommandBuffer &operator[](size_t index);

    };

} // vklite

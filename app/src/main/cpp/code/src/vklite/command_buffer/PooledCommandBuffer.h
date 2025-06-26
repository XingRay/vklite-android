//
// Created by leixing on 2025/5/16.
//

#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>
#include <vklite/device/Device.h>

#include "CommandBufferInterface.h"

namespace vklite {

    class PooledCommandBuffer : public CommandBufferInterface {
    private:
        vk::CommandBuffer mCommandBuffer;

    public:
        explicit PooledCommandBuffer(vk::CommandBuffer commandBuffer);

        ~PooledCommandBuffer() override;

        PooledCommandBuffer(const PooledCommandBuffer &other) = delete;

        PooledCommandBuffer &operator=(const PooledCommandBuffer &other) = delete;

        PooledCommandBuffer(PooledCommandBuffer &&other) noexcept;

        PooledCommandBuffer &operator=(PooledCommandBuffer &&other) noexcept;

        [[nodiscard]]
        const vk::CommandBuffer &getVkCommandBuffer() const override;
    };

} // vklite

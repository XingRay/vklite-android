//
// Created by leixing on 2024/12/30.
//

#pragma once

#include <functional>
#include <vector>

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/command_buffer/CommandBuffers.h"

namespace vklite {

    class CommandPool {
    private:
        const Device &mDevice;
        uint32_t mQueueFamilyIndex;

        vk::CommandPool mCommandPool;

    public:
        CommandPool(const Device &device, vk::CommandPoolCreateFlags flags, uint32_t queueFamilyIndex);

        ~CommandPool();

        [[nodiscard]]
        const vk::CommandPool &getCommandPool() const;

        [[nodiscard]]
        CommandBuffers allocateCommandBuffers(uint32_t count) const;

        [[nodiscard]]
        std::unique_ptr<CommandBuffers> allocateUniqueCommandBuffers(uint32_t count) const;

        void submitOneTimeCommand(const std::function<void(const vk::CommandBuffer &)> &command) const;
    };

} // vklite

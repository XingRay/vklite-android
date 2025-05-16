//
// Created by leixing on 2025/5/15.
//

#include "vklite/command_buffer/CommandBuffers.h"
#include "vklite/command_buffer/CommandPool.h"
#include "vklite/Log.h"

namespace vklite {

    CommandBuffers::CommandBuffers(const Device &device,
                                   const CommandPool &commandPool,
                                   std::vector<CommandBuffer> &&commandBuffers)
            : mDevice(device),
              mCommandPool(commandPool),
              mCommandBuffers(std::move(commandBuffers)) {

    }

    CommandBuffers::CommandBuffers(CommandBuffers &&other) noexcept
            : mDevice(other.mDevice),
              mCommandPool(other.mCommandPool),
              mCommandBuffers(std::move(other.mCommandBuffers)) {}

    CommandBuffers::~CommandBuffers() {
        if (!mCommandBuffers.empty()) {
            std::vector<vk::CommandBuffer> vkCommandBuffers;
            vkCommandBuffers.reserve(mCommandBuffers.size());
            for (const CommandBuffer &commandBuffer: mCommandBuffers) {
                vkCommandBuffers.push_back(commandBuffer.getCommandBuffer());
            }
            LOG_D("CommandBuffers::~CommandBuffers() freeCommandBuffers");
            mDevice.getDevice().freeCommandBuffers(mCommandPool.getCommandPool(), vkCommandBuffers);
        }
    }

    const CommandBuffer &CommandBuffers::operator[](size_t index) {
        return mCommandBuffers[index];
    }
} // vklite
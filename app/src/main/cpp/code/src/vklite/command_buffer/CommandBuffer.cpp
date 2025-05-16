//
// Created by leixing on 2025/5/16.
//

#include "CommandBuffer.h"
#include "vklite/command_buffer/CommandPool.h"

namespace vklite {

    CommandBuffer::CommandBuffer(const Device &device, const CommandPool &commandPool, const vk::CommandBuffer &commandBuffer)
            : mDevice(device), mCommandPool(commandPool), mCommandBuffer(commandBuffer) {}

    CommandBuffer::~CommandBuffer() = default;

    const vk::CommandBuffer &CommandBuffer::getCommandBuffer() const {
        return mCommandBuffer;
    }

} // vklite
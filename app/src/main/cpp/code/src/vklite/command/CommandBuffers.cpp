//
// Created by leixing on 2025/5/15.
//

#include "vklite/command/CommandBuffers.h"
#include "vklite/command/CommandPool.h"

namespace vklite {

    CommandBuffers::CommandBuffers(const Device &device,
                                   const CommandPool &commandPool,
                                   vk::CommandBufferLevel commandBufferLevel,
                                   uint32_t count)
            : mDevice(device),
              mCommandPool(commandPool) {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo{};
        commandBufferAllocateInfo
                .setLevel(commandBufferLevel)
                .setCommandPool(mCommandPool.getCommandPool())
                .setCommandBufferCount(count);

        mCommandBuffers = mDevice.getDevice().allocateCommandBuffers(commandBufferAllocateInfo);
    }

    CommandBuffers::CommandBuffers(CommandBuffers &&other) noexcept
            : mDevice(other.mDevice),
              mCommandPool(other.mCommandPool),
              mCommandBuffers(std::move(other.mCommandBuffers)) {}

    CommandBuffers::~CommandBuffers() {
        if (!mCommandBuffers.empty()) {
            mDevice.getDevice().freeCommandBuffers(mCommandPool.getCommandPool(), mCommandBuffers);
        }
    }

    const vk::CommandBuffer &CommandBuffers::operator[](size_t index) {
        return mCommandBuffers[index];
    }
} // vklite
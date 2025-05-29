//
// Created by leixing on 2025/5/15.
//

#include "vklite/command_buffer/CommandBuffers.h"
#include "vklite/command_pool/CommandPool.h"
#include "vklite/Log.h"

#include <utility>

namespace vklite {

    CommandBuffers::CommandBuffers(vk::Device device,
                                   vk::CommandPool commandPool,
                                   std::vector<vk::CommandBuffer> &&commandBuffers)
            : mDevice(device),
              mCommandPool(commandPool),
              mCommandBuffers(std::move(commandBuffers)) {
        mPooledCommandBuffers.reserve(mCommandBuffers.size());
        for (const vk::CommandBuffer &commandBuffer: mCommandBuffers) {
            mPooledCommandBuffers.emplace_back(commandBuffer);
        }
    }

    CommandBuffers::~CommandBuffers() {
        if (mDevice != nullptr && mCommandPool != nullptr && !mCommandBuffers.empty()) {
            mDevice.freeCommandBuffers(mCommandPool, mCommandBuffers);
            mDevice = nullptr;
            mCommandPool = nullptr;
            mCommandBuffers.clear();
            mPooledCommandBuffers.clear();
        }
    }

    CommandBuffers::CommandBuffers(CommandBuffers &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mCommandPool(std::exchange(other.mCommandPool, nullptr)),
              mCommandBuffers(std::move(other.mCommandBuffers)),
              mPooledCommandBuffers(std::move(other.mPooledCommandBuffers)) {}

    CommandBuffers &CommandBuffers::operator=(CommandBuffers &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mCommandPool = std::exchange(other.mCommandPool, nullptr);
            mCommandBuffers = std::move(other.mCommandBuffers);
            mPooledCommandBuffers = std::move(other.mPooledCommandBuffers);
        }
        return *this;
    }

    const PooledCommandBuffer &CommandBuffers::operator[](size_t index) {
        return mPooledCommandBuffers[index];
    }
} // vklite
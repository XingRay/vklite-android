//
// Created by leixing on 2025/5/30.
//

#include "CommandBuffer.h"
#include <utility>

namespace vklite {

    CommandBuffer::CommandBuffer(vk::Device device, vk::CommandPool commandPool, vk::CommandBuffer commandBuffer)
            : mDevice(device), mCommandPool(commandPool), mCommandBuffer(commandBuffer) {}

    CommandBuffer::~CommandBuffer() {
        if (mDevice != nullptr && mCommandPool != nullptr && mCommandBuffer != nullptr) {
            mDevice.freeCommandBuffers(mCommandPool, mCommandBuffer);
            mDevice = nullptr;
            mCommandPool = nullptr;
            mCommandBuffer = nullptr;
        }
    }

    CommandBuffer::CommandBuffer(CommandBuffer &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mCommandPool(std::exchange(other.mCommandPool, nullptr)),
              mCommandBuffer(std::exchange(other.mCommandBuffer, nullptr)) {}

    CommandBuffer &CommandBuffer::operator=(CommandBuffer &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mCommandPool = std::exchange(other.mCommandPool, nullptr);
            mCommandBuffer = std::exchange(other.mCommandBuffer, nullptr);
        }
        return *this;
    }

    const vk::CommandBuffer &CommandBuffer::getVkCommandBuffer() const {
        return mCommandBuffer;
    }

} // vklite
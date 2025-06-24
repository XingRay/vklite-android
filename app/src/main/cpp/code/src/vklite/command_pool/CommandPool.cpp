//
// Created by leixing on 2024/12/30.
//

#include "CommandPool.h"
#include "vklite/Log.h"

#include <utility>

namespace vklite {

    CommandPool::CommandPool(vk::Device device, vk::CommandPool commandPool, CommandPoolMeta meta)
            : mDevice(device), mCommandPool(commandPool), mMeta(meta) {}

    CommandPool::~CommandPool() {
        if (mDevice != nullptr && mCommandPool != nullptr) {
            mDevice.destroy(mCommandPool);
            mDevice = nullptr;
            mCommandPool = nullptr;
        }
    }

    CommandPool::CommandPool(CommandPool &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mCommandPool(std::exchange(other.mCommandPool, nullptr)),
              mMeta(other.mMeta) {}

    CommandPool &CommandPool::operator=(CommandPool &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mCommandPool = std::exchange(other.mCommandPool, nullptr);
            mMeta = other.mMeta;
        }
        return *this;
    }

    const vk::CommandPool &CommandPool::getCommandPool() const {
        return mCommandPool;
    }

    CommandBuffers CommandPool::allocate(uint32_t count, vk::CommandBufferLevel level) const {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo{};
        commandBufferAllocateInfo
                .setLevel(level)
                .setCommandPool(mCommandPool)
                .setCommandBufferCount(count);

        std::vector<vk::CommandBuffer> vkCommandBuffers = mDevice.allocateCommandBuffers(commandBufferAllocateInfo);
        return CommandBuffers{mDevice, mCommandPool, std::move(vkCommandBuffers)};
    }

    std::unique_ptr<CommandBuffers> CommandPool::allocateUnique(uint32_t count, vk::CommandBufferLevel level) const {
        return std::make_unique<CommandBuffers>(allocate(count, level));
    }

    CommandBuffer CommandPool::allocateOne(vk::CommandBufferLevel level) const {

        vk::CommandBufferAllocateInfo commandBufferAllocateInfo{};
        commandBufferAllocateInfo
                .setLevel(level)
                .setCommandPool(mCommandPool)
                .setCommandBufferCount(1);

        vk::CommandBuffer commandBuffer = mDevice.allocateCommandBuffers(commandBufferAllocateInfo)[0];
        return CommandBuffer{mDevice, mCommandPool, commandBuffer};
    }

    std::unique_ptr<CommandBuffer> CommandPool::allocateOneUnique(vk::CommandBufferLevel level) const {
        return std::make_unique<CommandBuffer>(allocateOne(level));
    }

    void CommandPool::submit(const vk::Queue &queue, const std::function<void(const vk::CommandBuffer &)> &command) const {

        CommandBuffer commandBuffer = allocateOne();
        commandBuffer.recordOneTimeSubmit(command);

        vk::SubmitInfo submitInfo{};
        submitInfo
                .setCommandBufferCount(1)
                .setPCommandBuffers(&(commandBuffer.getCommandBuffer()));

        queue.submit(submitInfo);
        queue.waitIdle();
    }

    void CommandPool::submit(const std::function<void(const vk::CommandBuffer &)> &command) const {
        vk::Queue queue = mDevice.getQueue(mMeta.queueFamilyIndex, 0);
        submit(queue, command);
    }

    void CommandPool::submit(const Queue &queue, const std::function<void(const vk::CommandBuffer &)> &command) const {

        CommandBuffer commandBuffer = allocateOne();
        commandBuffer.recordOneTimeSubmit(command);

        queue.submit(commandBuffer.getCommandBuffer());
        queue.waitIdle();
    }

} // vklite
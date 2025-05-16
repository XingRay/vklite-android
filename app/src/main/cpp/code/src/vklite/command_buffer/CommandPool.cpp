//
// Created by leixing on 2024/12/30.
//

#include "vklite/command_buffer/CommandPool.h"
#include "vklite/Log.h"

namespace vklite {

    CommandPool::CommandPool(const Device &device, vk::CommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
            : mDevice(device), mQueueFamilyIndex(queueFamilyIndex) {

        vk::CommandPoolCreateInfo commandPoolCreateInfo{};
        commandPoolCreateInfo
                .setFlags(flags)
                .setQueueFamilyIndex(queueFamilyIndex);

        mCommandPool = device.getDevice().createCommandPool(commandPoolCreateInfo);
    }

    CommandPool::~CommandPool() {
        LOG_D("CommandPool::~CommandPool()");
        mDevice.getDevice().destroy(mCommandPool);
    }

    const vk::CommandPool &CommandPool::getCommandPool() const {
        return mCommandPool;
    }

    CommandBuffers CommandPool::allocateCommandBuffers(uint32_t count) const {
//        return CommandBuffers(mDevice, *this, vk::CommandBufferLevel::ePrimary, count);
        return {mDevice, *this, vk::CommandBufferLevel::ePrimary, count};
    }

    std::unique_ptr<CommandBuffers> CommandPool::allocateUniqueCommandBuffers(uint32_t count) const {
        return std::make_unique<CommandBuffers>(mDevice, *this, vk::CommandBufferLevel::ePrimary, count);
    }

    void CommandPool::submitOneTimeCommand(const std::function<void(const vk::CommandBuffer &)> &command) const {

        vk::CommandBufferAllocateInfo commandBufferAllocateInfo{};
        commandBufferAllocateInfo
                .setLevel(vk::CommandBufferLevel::ePrimary)
                .setCommandPool(mCommandPool)
                .setCommandBufferCount(1);

        vk::CommandBuffer commandBuffer = mDevice.getDevice().allocateCommandBuffers(commandBufferAllocateInfo)[0];

        vk::CommandBufferBeginInfo commandBufferBeginInfo;
        commandBufferBeginInfo
                .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
                .setPInheritanceInfo(nullptr);

        commandBuffer.begin(commandBufferBeginInfo);

        command(commandBuffer);

        commandBuffer.end();

        vk::SubmitInfo submitInfo{};
        submitInfo.setCommandBufferCount(1)
                .setPCommandBuffers(&commandBuffer);

        vk::Queue queue = mDevice.getQueue(mQueueFamilyIndex);
        queue.submit(submitInfo);
        queue.waitIdle();

        mDevice.getDevice().freeCommandBuffers(mCommandPool, commandBuffer);
    }

} // vklite
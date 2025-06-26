//
// Created by leixing on 2025/5/30.
//

#include "CommandBufferInterface.h"

namespace vklite {

    CommandBufferInterface::CommandBufferInterface() = default;

    CommandBufferInterface::~CommandBufferInterface() = default;

    void CommandBufferInterface::record(const vk::CommandBufferBeginInfo &beginInfo, const std::function<void(const vk::CommandBuffer &commandBuffer)> &handler) const {
        const vk::CommandBuffer &commandBuffer = getVkCommandBuffer();
        commandBuffer.reset();
        commandBuffer.begin(beginInfo);

        handler(commandBuffer);

        commandBuffer.end();
    }

    void CommandBufferInterface::record(vk::CommandBufferUsageFlagBits usage, const std::function<void(const vk::CommandBuffer &commandBuffer)> &handler) const {
        vk::CommandBufferBeginInfo beginInfo;
        beginInfo
                .setFlags(usage)
                .setPInheritanceInfo(nullptr);

        record(beginInfo, handler);
    }

    void CommandBufferInterface::record(const std::function<void(const vk::CommandBuffer &commandBuffer)> &handler) const {
        record(vk::CommandBufferUsageFlagBits::eSimultaneousUse, handler);
    }

    void CommandBufferInterface::recordOneTimeSubmit(const std::function<void(const vk::CommandBuffer &commandBuffer)> &handler) const {
        record(vk::CommandBufferUsageFlagBits::eOneTimeSubmit, handler);
    }
}
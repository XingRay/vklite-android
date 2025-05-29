//
// Created by leixing on 2025/5/30.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class CommandBufferInterface {
    private:

    public:

        CommandBufferInterface();

        virtual ~CommandBufferInterface() = 0;

        [[nodiscard]]
        virtual const vk::CommandBuffer &getCommandBuffer() const = 0;

        void record(const vk::CommandBufferBeginInfo &beginInfo, const std::function<void(const vk::CommandBuffer &commandBuffer)> &handler) const;

        void record(vk::CommandBufferUsageFlagBits usage, const std::function<void(const vk::CommandBuffer &commandBuffer)> &handler) const;

        void record(const std::function<void(const vk::CommandBuffer &commandBuffer)> &handler) const;

        void recordOneTimeSubmit(const std::function<void(const vk::CommandBuffer &commandBuffer)> &handler) const;
    };

}
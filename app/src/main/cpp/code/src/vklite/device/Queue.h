//
// Created by leixing on 2025/5/27.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class Queue {
    private:
        vk::Queue mQueue;
    public:
        explicit Queue(vk::Queue queue);

        ~Queue();

        Queue(const Queue &other) = delete;

        Queue &operator=(const Queue &other) = delete;

        Queue(Queue &&other) noexcept;

        Queue &operator=(Queue &&other) noexcept;

        [[nodiscard]]
        const vk::Queue &getVkQueue() const;

        [[nodiscard]]
        vk::Result present(const vk::SwapchainKHR &swapchain, uint32_t imageIndex, const vk::Semaphore &waitSemaphore) const;

        void submit(const vk::CommandBuffer &commandBuffer, const vk::Semaphore &signalSemaphore, const vk::Fence &fence) const;

        void submit(const vk::CommandBuffer &commandBuffer, vk::PipelineStageFlags waitStage, const vk::Semaphore &waitSemaphore, const vk::Semaphore &signalSemaphore, const vk::Fence &fence) const;

        void submit(const vk::CommandBuffer &commandBuffer) const;

        void waitIdle() const;
    };

} // vklite

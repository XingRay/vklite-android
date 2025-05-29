//
// Created by leixing on 2025/5/27.
//

#include "Queue.h"

#include <utility>

namespace vklite {

    Queue::Queue(vk::Queue queue)
            : mQueue(queue) {}

    Queue::~Queue() = default;

    Queue::Queue(Queue &&other) noexcept
            : mQueue(std::exchange(other.mQueue, nullptr)) {}

    Queue &Queue::operator=(Queue &&other) noexcept {
        if (this != &other) {
            mQueue = std::exchange(other.mQueue, nullptr);
        }
        return *this;
    }

    const vk::Queue &Queue::getQueue() const {
        return mQueue;
    }

    vk::Result Queue::present(vk::SwapchainKHR swapchain, uint32_t imageIndex, vk::Semaphore waitSemaphore) const {
        std::array<vk::SwapchainKHR, 1> swapChains = {swapchain};
        std::array<uint32_t, 1> imageIndices = {imageIndex};
        std::array<vk::Semaphore, 1> signalSemaphores = {waitSemaphore};
        vk::PresentInfoKHR presentInfo{};
        presentInfo
                .setWaitSemaphores(signalSemaphores)
                .setSwapchains(swapChains)
                .setImageIndices(imageIndices);

        vk::Result result;
        // https://github.com/KhronosGroup/Vulkan-Hpp/issues/599
        // 当出现图片不匹配时， cpp风格的 presentKHR 会抛出异常， 而不是返回 result， 而C风格的 presentKHR 接口会返回 result
        try {
            result = mQueue.presentKHR(presentInfo);
        } catch (const vk::OutOfDateKHRError &e) {
            result = vk::Result::eErrorOutOfDateKHR;
        }

        return result;
    }

    void Queue::submit(const vk::CommandBuffer &commandBuffer, const vk::Semaphore &signalSemaphore, const vk::Fence &fence) const {
        std::array<vk::CommandBuffer, 1> computeCommandBuffers = {commandBuffer};
        std::array<vk::Semaphore, 1> computeSignalSemaphores = {signalSemaphore};

        vk::SubmitInfo submitInfo{};
        submitInfo
                .setCommandBuffers(computeCommandBuffers)
                .setSignalSemaphores(computeSignalSemaphores);

        std::array<vk::SubmitInfo, 1> submitInfos = {submitInfo};

        mQueue.submit(submitInfos, fence);
    }

    void Queue::submit(const vk::CommandBuffer &commandBuffer, vk::PipelineStageFlags waitStage, const vk::Semaphore &waitSemaphore, const vk::Semaphore &signalSemaphore, vk::Fence fence) const {
        std::array<vk::CommandBuffer, 1> commandBuffers = {commandBuffer};
        std::array<vk::PipelineStageFlags, 1> waitStages = {waitStage};
        std::array<vk::Semaphore, 1> waitSemaphores = {waitSemaphore};
        std::array<vk::Semaphore, 1> signalSemaphores = {signalSemaphore};

        vk::SubmitInfo submitInfo{};
        submitInfo
                .setWaitSemaphores(waitSemaphores)
                .setWaitDstStageMask(waitStages)
                .setCommandBuffers(commandBuffers)
                .setSignalSemaphores(signalSemaphores);

        std::array<vk::SubmitInfo, 1> submitInfos = {submitInfo};

        mQueue.submit(submitInfos, fence);
    }

    void Queue::submit(const vk::CommandBuffer &commandBuffer) const {

        vk::SubmitInfo submitInfo{};
        submitInfo
                .setCommandBufferCount(1)
                .setPCommandBuffers(&commandBuffer);

        mQueue.submit(submitInfo);
    }

    void Queue::waitIdle() const {
        mQueue.waitIdle();
    }

} // vklite
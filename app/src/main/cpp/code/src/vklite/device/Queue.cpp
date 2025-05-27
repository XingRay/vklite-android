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

} // vklite
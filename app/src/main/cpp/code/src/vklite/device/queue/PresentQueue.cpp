//
// Created by leixing on 2025/5/16.
//

#include "PresentQueue.h"

namespace vklite {
    PresentQueue::PresentQueue(vk::Queue queue, uint32_t queueFamilyIndex, uint32_t queueIndex)
            : mQueue(queue), mQueueFamilyIndex(queueFamilyIndex), mQueueIndex(queueIndex) {}

    PresentQueue::~PresentQueue() = default;

} // vklite
//
// Created by leixing on 2025/5/16.
//

#include "GraphicQueue.h"

namespace vklite {

    GraphicQueue::GraphicQueue(vk::Queue queue, uint32_t queueFamilyIndex, uint32_t queueIndex)
            : mQueue(queue), mQueueFamilyIndex(queueFamilyIndex), mQueueIndex(queueIndex) {}

    GraphicQueue::~GraphicQueue() = default;

} // vklite
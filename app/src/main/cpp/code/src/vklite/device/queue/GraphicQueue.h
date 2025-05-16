//
// Created by leixing on 2025/5/16.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class GraphicQueue {
    private:
        vk::Queue mQueue;
        uint32_t mQueueFamilyIndex;
        uint32_t mQueueIndex;

    public:
        GraphicQueue(vk::Queue queue,
                     uint32_t queueFamilyIndex,
                     uint32_t queueIndex);

        ~GraphicQueue();
    };

} // vklite

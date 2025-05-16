//
// Created by leixing on 2025/5/16.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class PresentQueue {
    private:
        vk::Queue mQueue;
        uint32_t mQueueFamilyIndex;
        uint32_t mQueueIndex;

    public:
        PresentQueue(vk::Queue queue,
                     uint32_t queueFamilyIndex,
                     uint32_t queueIndex);

        ~PresentQueue();
    };

} // vklite

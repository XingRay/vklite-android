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
    };

} // vklite

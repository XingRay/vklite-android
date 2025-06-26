//
// Created by leixing on 2025/5/28.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class Semaphore {
    private:
        vk::Device mDevice;
        vk::Semaphore mSemaphore;

    public:
        explicit Semaphore(vk::Device device, vk::Semaphore semaphore);

        ~Semaphore();

        Semaphore(const Semaphore &other) = delete;

        Semaphore &operator=(const Semaphore &other) = delete;

        Semaphore(Semaphore &&other) noexcept;

        Semaphore &operator=(Semaphore &&other) noexcept;

        [[nodiscard]]
        const vk::Semaphore &getVkSemaphore() const;
    };

} // vklite

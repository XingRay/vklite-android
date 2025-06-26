//
// Created by leixing on 2025/5/28.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class Fence {
    private:
        vk::Device mDevice;
        vk::Fence mFence;

    public:
        explicit Fence(vk::Device device, vk::Fence fence);

        ~Fence();

        Fence(const Fence &other) = delete;

        Fence &operator=(const Fence &other) = delete;

        Fence(Fence &&other) noexcept;

        Fence &operator=(Fence &&other) noexcept;

        [[nodiscard]]
        const vk::Fence &getVkFence() const;

        vk::Result reset();

        vk::Result wait(uint64_t timeout);

        vk::Result wait();
    };

} // vklite

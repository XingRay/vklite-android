//
// Created by leixing on 2024/12/22.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class Surface {
        vk::Instance mInstance;
        vk::SurfaceKHR mSurface;

    public:

        explicit Surface(vk::Instance instance, vk::SurfaceKHR surface);

        ~Surface();

        Surface(const Surface &other) = delete;

        Surface &operator=(const Surface &other) = delete;

        Surface(Surface &&other) noexcept;

        Surface &operator=(Surface &&other) noexcept;

        [[nodiscard]]
        const vk::SurfaceKHR &getVkSurface() const;
    };

} // namespace engine

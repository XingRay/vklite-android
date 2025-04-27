//
// Created by leixing on 2024/12/22.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace vklite {

    class Surface {
    public:
        virtual ~Surface() = default;

        // 禁用拷贝和赋值操作符
        Surface(const Surface &) = delete;
        Surface &operator=(const Surface &) = delete;

        // 获取 Vulkan Surface
        [[nodiscard]]
        virtual vk::SurfaceKHR getSurface() const = 0;

    protected:
        Surface() = default;
    };

} // namespace engine

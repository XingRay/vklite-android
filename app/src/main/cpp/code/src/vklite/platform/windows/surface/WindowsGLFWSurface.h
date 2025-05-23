//
// Created by leixing on 2025/1/2.
//

#pragma once
#ifdef WIN32

#include <functional>

#include <vulkan/vulkan.hpp>

#include "vklite/vulkan_wrapper/VulkanSurface.h"
#include "vklite/vulkan_wrapper/Instance.h"

// That way GLFW will include its own definitions and automatically load the Vulkan header with it.
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace engine{
    class WindowsGLFWSurface : public VulkanSurface {

    private:
        vk::Instance mInstance;
        vk::SurfaceKHR mSurface;

    public:
        WindowsGLFWSurface(const vk::Instance& instance, GLFWwindow *window);

        ~WindowsGLFWSurface() override;

        [[nodiscard]]
        vk::SurfaceKHR getSurface() const override { return mSurface; }

        static std::function<std::unique_ptr<VulkanSurface>(const Instance &)> surfaceBuilder(GLFWwindow *window);
    };
}


#endif // WIN32

//
// Created by leixing on 2025/1/2.
//
#ifdef WIN32

#include "vklite/vulkan_wrapper/platform/windows/WindowsGLFWSurface.h"
#include "vklite/Log.h"

namespace vklite {
    WindowsGLFWSurface::WindowsGLFWSurface(const vk::Instance &instance, GLFWwindow *window) : mInstance(instance) {
        LOG_D("WindowsGLFWSurface::WindowsGLFWSurface");
        VkResult result = glfwCreateWindowSurface(instance, window, nullptr, reinterpret_cast<VkSurfaceKHR *>(&mSurface));
        if (result != VK_SUCCESS) {
            LOG_E("failed to create surface on windows !");
            throw std::runtime_error("failed to create surface on windows !");
        }
    }

    WindowsGLFWSurface::~WindowsGLFWSurface() {
        LOG_D("WindowsGLFWSurface::~WindowsGLFWSurface()");
        if (mSurface != nullptr) {
            mInstance.destroy(mSurface);
        } else {
            LOG_W("surface is null");
        }
    }

    std::function<std::unique_ptr<VulkanSurface>(const Instance &)> WindowsGLFWSurface::surfaceBuilder(GLFWwindow *window) {
        return [window](const Instance &instance) -> std::unique_ptr<VulkanSurface> {
            // 创建 WindowsGLFWSurface 实例
            return std::make_unique<WindowsGLFWSurface>(instance.getInstance(), window);
        };
    }
}

#endif // WIN32

//
// Created by leixing on 2024/12/17.
//

#pragma once

#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include "vklite/Log.h"
#include "VkResultUtil.h"

// C++ 风格的 Vulkan 调用包装函数
inline void CallVulkan(const vk::Result &result, const char *file, int line) {
    if (result != vk::Result::eSuccess) {
        std::string error = std::format("Vulkan error. {}, {}, File[ {} ], line[ {} ]", vklite::VkResultToString((VkResult) result), vklite::VkResultDescription((VkResult) result), file, line);
        LOG_E("%s", error.c_str());
        throw std::runtime_error(error);
    }
}

// 宏用于自动传递文件名和行号
#define CALL_VK_CPP(func) CallVulkan((func), __FILE__, __LINE__)

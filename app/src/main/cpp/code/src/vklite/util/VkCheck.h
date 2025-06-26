//
// Created by one on 2024/12/10.
//

#pragma once

#include "vklite/Log.h"
#include "VkResultUtil.h"

#define CALL_VK(func)                                                 \
  do {                                                                \
    VkResult __result = (func);                                       \
    if (__result != VK_SUCCESS) {                                     \
      const char* errorName = VkResultToString(__result);             \
      LOG_E("Vulkan error [%s]: %s. File[%s], line[%d]",              \
            errorName, VkResultDescription(__result),                 \
            __FILE__, __LINE__);                                      \
      assert(false);                                                  \
    }                                                                 \
  } while (0)
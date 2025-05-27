//
// Created by leixing on 2025/5/28.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
    extern PFN_vkCreateInstance vkCreateInstance;

    class InstanceApi {
    public:
        static void initInstanceApi(const vk::DynamicLoader &dynamicLoader);
    };


} // vklite
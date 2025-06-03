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

        static std::vector<vk::ExtensionProperties> enumerateInstanceExtensionProperties();

        static std::vector<const char *> enumerateInstanceExtensionNames();

        static std::vector<vk::LayerProperties> enumerateInstanceLayerProperties();

        static std::vector<const char *> enumerateInstanceLayerNames();

        static vk::Instance createInstance(const vk::InstanceCreateInfo &instanceCreateInfo);
    };


} // vklite
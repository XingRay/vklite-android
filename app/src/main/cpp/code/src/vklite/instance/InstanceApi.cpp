//
// Created by leixing on 2025/5/28.
//

#include "InstanceApi.h"

namespace vklite {

    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
    PFN_vkCreateInstance vkCreateInstance;

    void InstanceApi::initInstanceApi(const vk::DynamicLoader& dynamicLoader) {

        vkGetInstanceProcAddr = dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
        if (vkGetInstanceProcAddr == nullptr) {
            throw std::runtime_error("Failed to load vkGetInstanceProcAddr");
        }

        vkEnumerateInstanceExtensionProperties = dynamicLoader.getProcAddress<PFN_vkEnumerateInstanceExtensionProperties>("vkEnumerateInstanceExtensionProperties");
        if (vkEnumerateInstanceExtensionProperties == nullptr) {
            throw std::runtime_error("Failed to load vkEnumerateInstanceExtensionProperties");
        }

        vkEnumerateInstanceLayerProperties = dynamicLoader.getProcAddress<PFN_vkEnumerateInstanceLayerProperties>("vkEnumerateInstanceLayerProperties");
        if (vkEnumerateInstanceLayerProperties == nullptr) {
            throw std::runtime_error("Failed to load vkEnumerateInstanceLayerProperties");
        }

        vkCreateInstance = dynamicLoader.getProcAddress<PFN_vkCreateInstance>("vkCreateInstance");
        if (vkCreateInstance == nullptr) {
            throw std::runtime_error("Failed to load vkCreateInstance");
        }
    }

} // vklite
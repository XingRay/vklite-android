//
// Created by leixing on 2025/5/28.
//

#include "InstanceApi.h"

#include "vklite/util/VkCheck.h"
#include "vklite/util/VkCheckCpp.h"
#include "vklite/Log.h"

namespace vklite {

    bool mInited = false;

    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
    PFN_vkCreateInstance vkCreateInstance;

    void InstanceApi::initInstanceApi(const vk::DynamicLoader &dynamicLoader) {
        if (mInited) {
            return;
        }

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

        mInited = true;
    }


    std::vector<vk::ExtensionProperties> InstanceApi::enumerateInstanceExtensionProperties() {
        uint32_t extensionCount = 0;
        CALL_VK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr));

        std::vector<vk::ExtensionProperties> extensions(extensionCount);
        CALL_VK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, (VkExtensionProperties *) extensions.data()));

        return extensions;
    }

    std::vector<const char *> InstanceApi::enumerateInstanceExtensionNames() {
        std::vector<vk::ExtensionProperties> extensions = InstanceApi::enumerateInstanceExtensionProperties();

        std::vector<const char *> names;
        names.reserve(extensions.size());
        for (const auto &extensionProperties: extensions) {
            names.push_back(extensionProperties.extensionName);
        }

        return names;
    }

    std::vector<vk::LayerProperties> InstanceApi::enumerateInstanceLayerProperties() {
        uint32_t layerCount = 0;
        CALL_VK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

        std::vector<vk::LayerProperties> layers(layerCount);
        CALL_VK(vkEnumerateInstanceLayerProperties(&layerCount, (VkLayerProperties *) layers.data()));

        return layers;
    }

    std::vector<const char *> InstanceApi::enumerateInstanceLayerNames() {
        std::vector<vk::LayerProperties> layers = InstanceApi::enumerateInstanceLayerProperties();

        std::vector<const char *> names;
        for (const auto &layerProperties: layers) {
            LOG_D("  %s", layerProperties.layerName.data());
            names.push_back(layerProperties.layerName);
        }

        return names;
    }

    vk::Instance InstanceApi::createInstance(const vk::InstanceCreateInfo instanceCreateInfo) {
        vk::Instance instance;
        CALL_VK(vkCreateInstance((VkInstanceCreateInfo *) &instanceCreateInfo, nullptr, (VkInstance *) &instance));
        return instance;
    }

} // vklite
//
// Created by leixing on 2025/3/15.
//

#include "VulkanAndroidInstancePlugin.h"
#include "VulkanAndroidApi.h"

namespace vklite {

    VulkanAndroidInstancePlugin::VulkanAndroidInstancePlugin() = default;

    VulkanAndroidInstancePlugin::~VulkanAndroidInstancePlugin() = default;

    void VulkanAndroidInstancePlugin::onInstanceCreated(const Instance &instance) {
        const vk::Instance &vkInstance = instance.getInstance();

        vkGetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID) vkGetInstanceProcAddr(vkInstance, "vkGetAndroidHardwareBufferPropertiesANDROID");
        vkBindImageMemory2KHR = (PFN_vkBindImageMemory2KHR) vkGetInstanceProcAddr(vkInstance, "vkBindImageMemory2KHR");

        vkCreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion) vkGetInstanceProcAddr(vkInstance, "vkCreateSamplerYcbcrConversion");
        vkDestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion) vkGetInstanceProcAddr(vkInstance, "vkDestroySamplerYcbcrConversion");

        vkGetImageMemoryRequirements2KHR = (PFN_vkGetImageMemoryRequirements2KHR) vkGetInstanceProcAddr(vkInstance, "vkGetImageMemoryRequirements2KHR");
    }
} // vklite
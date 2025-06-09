//
// Created by leixing on 2025/3/17.
//

#include "VulkanAndroidApi.h"

namespace vklite {

    PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
    PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;

    PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
    PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;

    PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;

    void VulkanAndroidApi::initApi(const Instance &instance) {
        const vk::Instance &vkInstance = instance.getInstance();

        vkGetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID) vkGetInstanceProcAddr(vkInstance, "vkGetAndroidHardwareBufferPropertiesANDROID");
        vkBindImageMemory2KHR = (PFN_vkBindImageMemory2KHR) vkGetInstanceProcAddr(vkInstance, "vkBindImageMemory2KHR");

        vkCreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion) vkGetInstanceProcAddr(vkInstance, "vkCreateSamplerYcbcrConversion");
        vkDestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion) vkGetInstanceProcAddr(vkInstance, "vkDestroySamplerYcbcrConversion");

        vkGetImageMemoryRequirements2KHR = (PFN_vkGetImageMemoryRequirements2KHR) vkGetInstanceProcAddr(vkInstance, "vkGetImageMemoryRequirements2KHR");
    }

} // vklite
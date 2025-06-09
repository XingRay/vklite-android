//
// Created by leixing on 2025/3/17.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/instance/Instance.h"

namespace vklite {

    extern PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
    extern PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;

    extern PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
    extern PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;

    extern PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;

    class VulkanAndroidApi{
    public:
        static void initApi(const Instance &instance);
    };

} // vklite

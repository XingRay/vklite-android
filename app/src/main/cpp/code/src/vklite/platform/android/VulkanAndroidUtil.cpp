//
// Created by leixing on 2025/3/9.
//

#include "VulkanAndroidUtil.h"
#include "vklite/util/VkCheck.h"

namespace vklite {

    vk::AndroidHardwareBufferFormatPropertiesANDROID VulkanAndroidUtil::getAndroidHardwareBufferFormat(vk::Device vkDevice, AHardwareBuffer* hardwareBuffer){
        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
        propertiesInfo.pNext = &formatInfo;

        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(vkDevice, hardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&propertiesInfo)));
        return formatInfo;
    }
} // vklite
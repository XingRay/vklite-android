//
// Created by leixing on 2025/4/28.
//

#include "AndroidDevicePlugin.h"

namespace vklite {

    AndroidDevicePlugin::AndroidDevicePlugin() {
        mPhysicalDeviceFeatures2.pNext = &mPhysicalDeviceSamplerYcbcrConversionFeatures;
        mPhysicalDeviceSamplerYcbcrConversionFeatures.samplerYcbcrConversion = true;
        mPhysicalDeviceFeatures2.features.samplerAnisotropy = true;
    }

    AndroidDevicePlugin::~AndroidDevicePlugin() = default;

    std::vector<const char *> AndroidDevicePlugin::getInstanceExtensions() {
        // todo:
        return {};
    }

    std::vector<const char *> AndroidDevicePlugin::getDeviceExtensions() {
        // todo:
        return {};
    }

    std::vector<const char *> AndroidDevicePlugin::getLayers() {
        // todo:
        return {};
    }

    void AndroidDevicePlugin::onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {
        deviceCreateInfo.setPNext(&mPhysicalDeviceFeatures2);
    }

} // vklite
//
// Created by leixing on 2025/4/28.
//

#include "AndroidPlugin.h"

namespace vklite {

    AndroidPlugin::AndroidPlugin() {
        mPhysicalDeviceFeatures2.pNext = &mPhysicalDeviceSamplerYcbcrConversionFeatures;
        mPhysicalDeviceSamplerYcbcrConversionFeatures.samplerYcbcrConversion = true;
        mPhysicalDeviceFeatures2.features.samplerAnisotropy = true;
    }

    AndroidPlugin::~AndroidPlugin() = default;

    std::vector<const char *> AndroidPlugin::getInstanceExtensions() {
        // todo:
        return {};
    }

    std::vector<const char *> AndroidPlugin::getDeviceExtensions() {
        // todo:
        return {};
    }

    std::vector<const char *> AndroidPlugin::getLayers() {
        // todo:
        return {};
    }

    void AndroidPlugin::onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {
        deviceCreateInfo.setPNext(&mPhysicalDeviceFeatures2);
    }

} // vklite
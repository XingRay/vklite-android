//
// Created by leixing on 2025/4/28.
//

#include "HardwareBufferDevicePlugin.h"

namespace vklite {

    HardwareBufferDevicePlugin::HardwareBufferDevicePlugin() {
        mPhysicalDeviceFeatures2.pNext = &mPhysicalDeviceSamplerYcbcrConversionFeatures;
        mPhysicalDeviceSamplerYcbcrConversionFeatures.samplerYcbcrConversion = true;
        mPhysicalDeviceFeatures2.features.samplerAnisotropy = true;
    }

    HardwareBufferDevicePlugin::~HardwareBufferDevicePlugin() = default;

//    std::vector<const char *> HardwareBufferDevicePlugin::getInstanceExtensions() {
//        // todo:
//        return {};
//    }
//
//    std::vector<const char *> HardwareBufferDevicePlugin::getDeviceExtensions() {
//        // todo:
//        return {};
//    }
//
//    std::vector<const char *> HardwareBufferDevicePlugin::getLayers() {
//        // todo:
//        return {};
//    }
//
//    void HardwareBufferDevicePlugin::onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {
//        deviceCreateInfo.setPNext(&mPhysicalDeviceFeatures2);
//    }

    std::vector<const char *> HardwareBufferDevicePlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> HardwareBufferDevicePlugin::getLayers() {
        return {};
    }

    void HardwareBufferDevicePlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {

    }

    void HardwareBufferDevicePlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {

    }

} // vklite
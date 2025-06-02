//
// Created by leixing on 2025/5/29.
//

#include "HardwareBufferPlugin.h"

namespace vklite {

    HardwareBufferPlugin::HardwareBufferPlugin() {
        mPhysicalDeviceFeatures2.pNext = &mPhysicalDeviceSamplerYcbcrConversionFeatures;
        mPhysicalDeviceSamplerYcbcrConversionFeatures.samplerYcbcrConversion = true;
        mPhysicalDeviceFeatures2.features.samplerAnisotropy = true;
    }

    HardwareBufferPlugin::~HardwareBufferPlugin() = default;

    std::vector<const char *> HardwareBufferPlugin::getInstanceExtensions() {
        return {};
    }

    std::vector<const char *> HardwareBufferPlugin::getInstanceLayers() {
        return {};
    }

    void HardwareBufferPlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {

    }

    void HardwareBufferPlugin::onInstanceCreated(Instance &instance) {

    }


    std::vector<const char *> HardwareBufferPlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> HardwareBufferPlugin::getDeviceLayers() {
        return {};
    }

    void HardwareBufferPlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {

    }

    void HardwareBufferPlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {

    }

    /*
     *
     * static methods
     *
     */
    std::unique_ptr<HardwareBufferPlugin> HardwareBufferPlugin::buildUnique() {
        std::unique_ptr<HardwareBufferPlugin> plugin = std::make_unique<HardwareBufferPlugin>();

        return std::move(plugin);
    }

} // vklite
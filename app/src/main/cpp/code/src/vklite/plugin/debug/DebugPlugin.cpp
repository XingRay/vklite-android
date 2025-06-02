//
// Created by leixing on 2025/6/3.
//

#include "DebugPlugin.h"

namespace vklite {

    DebugPlugin::DebugPlugin() = default;

    DebugPlugin::~DebugPlugin() = default;

    std::vector<const char *> DebugPlugin::getInstanceExtensions() {
        return {};
    }

    std::vector<const char *> DebugPlugin::getInstanceLayers() {
        return {};
    }

    void DebugPlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {

    }

    void DebugPlugin::onInstanceCreated(Instance &instance) {

    }


    std::vector<const char *> DebugPlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> DebugPlugin::getDeviceLayers() {
        return {};
    }

    void DebugPlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {

    }

    void DebugPlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {

    }
    
} // vklite
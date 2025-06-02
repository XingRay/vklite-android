//
// Created by leixing on 2025/6/3.
//

#include "AndroidSurfacePlugin.h"

namespace vklite {

    AndroidSurfacePlugin::AndroidSurfacePlugin() = default;

    AndroidSurfacePlugin::~AndroidSurfacePlugin() = default;

    std::vector<const char *> AndroidSurfacePlugin::getInstanceExtensions() {
        return {};
    }

    std::vector<const char *> AndroidSurfacePlugin::getInstanceLayers() {
        return {};
    }

    void AndroidSurfacePlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {

    }

    void AndroidSurfacePlugin::onInstanceCreated(Instance &instance) {

    }


    std::vector<const char *> AndroidSurfacePlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> AndroidSurfacePlugin::getDeviceLayers() {
        return {};
    }

    void AndroidSurfacePlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {

    }

    void AndroidSurfacePlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {

    }

} // vklite
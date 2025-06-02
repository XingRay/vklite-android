//
// Created by leixing on 2025/6/3.
//

#include "SurfacePlugin.h"

namespace vklite {

    SurfacePlugin::SurfacePlugin() = default;

    SurfacePlugin::~SurfacePlugin() = default;

    std::vector<const char *> SurfacePlugin::getInstanceExtensions() {
        return {};
    }

    std::vector<const char *> SurfacePlugin::getInstanceLayers() {
        return {};
    }

    void SurfacePlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {

    }

    void SurfacePlugin::onInstanceCreated(Instance &instance) {

    }


    std::vector<const char *> SurfacePlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> SurfacePlugin::getDeviceLayers() {
        return {};
    }

    void SurfacePlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {

    }

    void SurfacePlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {

    }

} // vklite
//
// Created by leixing on 2025/6/3.
//

#include "AndroidSurfacePlugin.h"

#include "vklite/platform/android/plugin/AndroidPlugin.h"
#include "vklite/plugin/surface/SurfacePlugin.h"

namespace vklite {

    AndroidSurfacePlugin::AndroidSurfacePlugin() = default;

    AndroidSurfacePlugin::~AndroidSurfacePlugin() = default;

    std::vector<const char *> AndroidSurfacePlugin::getInstanceExtensions() {
        return {
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
        };
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

    /*
     *
     * static methods
     *
     */
    std::unique_ptr<AndroidSurfacePlugin> AndroidSurfacePlugin::buildUnique() {
        std::unique_ptr<AndroidSurfacePlugin> plugin = std::make_unique<AndroidSurfacePlugin>();

        return std::move(plugin);
    }

    std::unique_ptr<CombinedPlugin> AndroidSurfacePlugin::buildUniqueCombined() {
        std::unique_ptr<CombinedPlugin> combinedPlugin = CombinedPlugin::buildUnique();
        combinedPlugin->addDependency(SurfacePlugin::buildUnique());
        combinedPlugin->addDependency(AndroidPlugin::buildUnique());
        combinedPlugin->addDependency(buildUnique());

        return std::move(combinedPlugin);
    }

} // vklite
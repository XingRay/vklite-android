//
// Created by leixing on 2025/6/3.
//

#include "AndroidPlugin.h"
#include "vklite/platform/android/instance/VulkanAndroidApi.h"

namespace vklite {

    AndroidPlugin::AndroidPlugin() = default;

    AndroidPlugin::~AndroidPlugin() = default;

    std::vector<const char *> AndroidPlugin::getInstanceExtensions() {
        return {};
    }

    std::vector<const char *> AndroidPlugin::getInstanceLayers() {
        return {};
    }

    void AndroidPlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {

    }

    void AndroidPlugin::onInstanceCreated(Instance &instance) {
        VulkanAndroidApi::initApi(instance);
    }


    std::vector<const char *> AndroidPlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> AndroidPlugin::getDeviceLayers() {
        return {};
    }

    void AndroidPlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {

    }

    void AndroidPlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {

    }

    /*
     *
     * static methods
     *
     */
    std::unique_ptr<AndroidPlugin> AndroidPlugin::buildUnique() {
        std::unique_ptr<AndroidPlugin> plugin = std::make_unique<AndroidPlugin>();

        return std::move(plugin);
    }

} // vklite
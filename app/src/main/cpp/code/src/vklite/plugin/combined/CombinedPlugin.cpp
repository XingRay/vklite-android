//
// Created by leixing on 2025/6/3.
//

#include "CombinedPlugin.h"

namespace vklite {

    CombinedPlugin::CombinedPlugin() = default;

    CombinedPlugin::~CombinedPlugin() = default;

    std::vector<const char *> CombinedPlugin::getInstanceExtensions() {
        std::vector<const char *> instanceExtensions;

        for (const std::unique_ptr<PluginInterface> &dependency: mDependencies) {
            std::vector<const char *> dependencyInstanceExtensions = dependency->getInstanceExtensions();
            instanceExtensions.insert(instanceExtensions.begin(), std::move_iterator(dependencyInstanceExtensions.begin()), std::move_iterator(dependencyInstanceExtensions.end()));
        }

        return instanceExtensions;
    }

    std::vector<const char *> CombinedPlugin::getInstanceLayers() {
        std::vector<const char *> instanceLayers;

        for (const std::unique_ptr<PluginInterface> &dependency: mDependencies) {
            std::vector<const char *> dependencyInstanceLayers = dependency->getInstanceLayers();
            instanceLayers.insert(instanceLayers.begin(), std::move_iterator(dependencyInstanceLayers.begin()), std::move_iterator(dependencyInstanceLayers.end()));
        }

        return instanceLayers;
    }

    void CombinedPlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {
        for (const std::unique_ptr<PluginInterface> &dependency: mDependencies) {
            dependency->onPreCreateInstance(instanceCreateInfo);
        }
    }

    void CombinedPlugin::onInstanceCreated(Instance &instance) {
        for (const std::unique_ptr<PluginInterface> &dependency: mDependencies) {
            dependency->onInstanceCreated(instance);
        }
    }


    std::vector<const char *> CombinedPlugin::getDeviceExtensions() {
        std::vector<const char *> instanceLayers;

        for (const std::unique_ptr<PluginInterface> &dependency: mDependencies) {
            std::vector<const char *> dependencyInstanceLayers = dependency->getDeviceExtensions();
            instanceLayers.insert(instanceLayers.begin(), std::move_iterator(dependencyInstanceLayers.begin()), std::move_iterator(dependencyInstanceLayers.end()));
        }

        return instanceLayers;
    }

    std::vector<const char *> CombinedPlugin::getDeviceLayers() {
        std::vector<const char *> deviceLayers;

        for (const std::unique_ptr<PluginInterface> &dependency: mDependencies) {
            std::vector<const char *> dependencyDeviceLayers = dependency->getDeviceLayers();
            deviceLayers.insert(deviceLayers.begin(), std::move_iterator(dependencyDeviceLayers.begin()), std::move_iterator(dependencyDeviceLayers.end()));
        }

        return deviceLayers;
    }

    void CombinedPlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {
        for (const std::unique_ptr<PluginInterface> &dependency: mDependencies) {
            dependency->physicalDeviceFeaturesConfigure(physicalDeviceFeatures);
        }
    }

    void CombinedPlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {
        for (const std::unique_ptr<PluginInterface> &dependency: mDependencies) {
            dependency->onPreCreateDevice(deviceCreateInfo);
        }
    }

    CombinedPlugin &CombinedPlugin::addDependency(std::unique_ptr<PluginInterface> plugin) {
        mDependencies.push_back(std::move(plugin));
        return *this;
    }

    /*
     *
     * static methods
     *
     */
    std::unique_ptr<CombinedPlugin> CombinedPlugin::buildUnique() {
        std::unique_ptr<CombinedPlugin> plugin = std::make_unique<CombinedPlugin>();

        return std::move(plugin);
    }

} // vklite
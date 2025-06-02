//
// Created by leixing on 2025/6/3.
//

#include "DebugPlugin.h"

namespace vklite {

    DebugPlugin::DebugPlugin() = default;

    DebugPlugin::~DebugPlugin() = default;

    std::vector<const char *> DebugPlugin::getInstanceExtensions() {
        return {
                // old version
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                // new version
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };
    }

    std::vector<const char *> DebugPlugin::getInstanceLayers() {
        return {
                "VK_LAYER_KHRONOS_validation"
        };
    }

    void DebugPlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {
//        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
//        if (mEnableValidationLayer) {
//            if (!checkValidationLayerSupported()) {
//                throw std::runtime_error("validation layers required, but not available !");
//            }
//            createInfo.setPEnabledLayerNames(mValidationLayers);

//        populateDebugMessengerCreateInfo(debugCreateInfo);
//        instanceCreateInfo.pNext = &debugCreateInfo;
//        } else {
//            createInfo.enabledLayerCount = 0;
//            createInfo.pNext = nullptr;
//        }
    }

    void DebugPlugin::onInstanceCreated(Instance &instance) {

    }


    std::vector<const char *> DebugPlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> DebugPlugin::getDeviceLayers() {
        return {
                "VK_LAYER_KHRONOS_validation"
        };
    }

    void DebugPlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {

    }

    void DebugPlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {

    }

    /*
     *
     * static methods
     *
     */
    std::unique_ptr<DebugPlugin> DebugPlugin::buildUnique() {
        std::unique_ptr<DebugPlugin> plugin = std::make_unique<DebugPlugin>();

        return std::move(plugin);
    }

} // vklite
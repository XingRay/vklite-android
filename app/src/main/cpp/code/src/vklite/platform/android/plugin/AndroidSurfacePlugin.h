//
// Created by leixing on 2025/6/3.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/plugin/PluginInterface.h"
#include "vklite/instance/Instance.h"
#include "vklite/plugin/combined/CombinedPlugin.h"

namespace vklite {

    class AndroidSurfacePlugin : public PluginInterface {
    private:

    public:
        AndroidSurfacePlugin();

        ~AndroidSurfacePlugin() override;

        std::vector<const char *> getInstanceExtensions() override;

        std::vector<const char *> getInstanceLayers() override;

        void onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) override;

        void onInstanceCreated(Instance &instance) override;


        std::vector<const char *> getDeviceExtensions() override;

        std::vector<const char *> getDeviceLayers() override;

        void physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) override;

        void onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) override;

    public: // static
        static std::unique_ptr<AndroidSurfacePlugin> buildUnique();
        static std::unique_ptr<CombinedPlugin> buildUniqueCombined();
    };

} // vklite

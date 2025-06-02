//
// Created by leixing on 2025/6/3.
//

#pragma once

#include "vklite/plugin/PluginInterface.h"

namespace vklite {

    class SurfacePlugin : public PluginInterface {
    private:

    public:
        SurfacePlugin();

        ~SurfacePlugin() override;

        std::vector<const char *> getInstanceExtensions() override;

        std::vector<const char *> getInstanceLayers() override;

        void onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) override;

        void onInstanceCreated(Instance &instance) override;


        std::vector<const char *> getDeviceExtensions() override;

        std::vector<const char *> getDeviceLayers() override;

        void physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) override;

        void onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) override;

    };

} // vklite

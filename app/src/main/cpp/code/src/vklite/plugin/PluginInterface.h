//
// Created by leixing on 2025/4/28.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/instance/Instance.h"

namespace vklite {

    class PluginInterface {
    public:
        PluginInterface();

        virtual ~PluginInterface();

        [[nodiscard]]
        virtual std::vector<const char *> getInstanceExtensions() = 0;

        [[nodiscard]]
        virtual std::vector<const char *> getInstanceLayers() = 0;

        virtual void onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) = 0;

        virtual void onInstanceCreated(Instance &instance) = 0;


        [[nodiscard]]
        virtual std::vector<const char *> getDeviceExtensions() = 0;

        [[nodiscard]]
        virtual std::vector<const char *> getDeviceLayers() = 0;

        virtual void physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) = 0;

        virtual void onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) = 0;
    };

} // vklite
//
// Created by leixing on 2025/5/27.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class DevicePluginInterface {
    public:

        DevicePluginInterface();

        virtual ~DevicePluginInterface();

        virtual std::vector<const char *> getDeviceExtensions() = 0;

        virtual std::vector<const char *> getLayers() = 0;

        virtual void physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) = 0;

        virtual void onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) = 0;
    };

} // vklite

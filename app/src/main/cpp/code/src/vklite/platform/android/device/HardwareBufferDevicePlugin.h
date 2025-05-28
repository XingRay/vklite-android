//
// Created by leixing on 2025/4/28.
//

#pragma once

#include <vector>

#include "vklite/device/DevicePluginInterface.h"

namespace vklite {

    class HardwareBufferDevicePlugin : public DevicePluginInterface {
    private:
        vk::PhysicalDeviceFeatures2 mPhysicalDeviceFeatures2;
        vk::PhysicalDeviceSamplerYcbcrConversionFeatures mPhysicalDeviceSamplerYcbcrConversionFeatures;

    public:

        HardwareBufferDevicePlugin();

        ~HardwareBufferDevicePlugin() override;

//        std::vector<const char *> getInstanceExtensions();
//
//        std::vector<const char *> getDeviceExtensions();
//
//        std::vector<const char *> getLayers();
//
//        void onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo);

        std::vector<const char *> getDeviceExtensions() override;

        std::vector<const char *> getLayers() override;

        void physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) override;

        void onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) override;
    };

} // vklite

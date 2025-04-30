//
// Created by leixing on 2025/4/28.
//

#pragma once

#include "vklite/device/DevicePlugin.h"

namespace vklite {

    class AndroidDevicePlugin : public DevicePlugin {
    private:
        vk::PhysicalDeviceFeatures2 mPhysicalDeviceFeatures2;
        vk::PhysicalDeviceSamplerYcbcrConversionFeatures mPhysicalDeviceSamplerYcbcrConversionFeatures;

    public:

        AndroidDevicePlugin();

        ~AndroidDevicePlugin() override;

        std::vector<const char *> getInstanceExtensions() override;

        std::vector<const char *> getDeviceExtensions() override;

        std::vector<const char *> getLayers() override;

        void onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) override;
    };

} // vklite

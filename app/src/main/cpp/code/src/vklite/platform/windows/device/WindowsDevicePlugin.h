//
// Created by leixing on 2025/4/28.
//

#pragma once

#include "vklite/device/DevicePlugin.h"

namespace vklite {

    class WindowsDevicePlugin : public DevicePlugin {
    private:
    public:

        std::vector<const char *> getInstanceExtensions() override;

        std::vector<const char *> getDeviceExtensions() override;

        std::vector<const char *> getLayers() override;

        void onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) override;
    };

} // vklite

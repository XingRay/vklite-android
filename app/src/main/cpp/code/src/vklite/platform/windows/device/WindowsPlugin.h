//
// Created by leixing on 2025/4/28.
//

#pragma once

#include "vklite/plugin/PluginInterface.h"

namespace vklite {

    class WindowsPlugin {
    private:
    public:

        std::vector<const char *> getInstanceExtensions();

        std::vector<const char *> getDeviceExtensions();

        std::vector<const char *> getLayers();

        void onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo);
    };

} // vklite

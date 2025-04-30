//
// Created by leixing on 2025/4/28.
//

#include "WindowsDevicePlugin.h"

namespace vklite {

    std::vector<const char *> WindowsDevicePlugin::getInstanceExtensions() {
        return {};
    }

    std::vector<const char *> WindowsDevicePlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> WindowsDevicePlugin::getLayers() {
        return {};
    }

    void WindowsDevicePlugin::onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {

    }

} // vklite
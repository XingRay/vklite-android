//
// Created by leixing on 2025/4/28.
//

#include "WindowsPlugin.h"

namespace vklite {

    std::vector<const char *> WindowsPlugin::getInstanceExtensions() {
        return {};
    }

    std::vector<const char *> WindowsPlugin::getDeviceExtensions() {
        return {};
    }

    std::vector<const char *> WindowsPlugin::getLayers() {
        return {};
    }

    void WindowsPlugin::onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {

    }

} // vklite
//
// Created by leixing on 2025/5/28.
//

#include "HardwareBufferInstancePlugin.h"

namespace vklite {

    HardwareBufferInstancePlugin::HardwareBufferInstancePlugin() = default;

    HardwareBufferInstancePlugin::~HardwareBufferInstancePlugin() = default;

    std::vector<const char *> HardwareBufferInstancePlugin::getExtensions() {
        return {};
    }

    std::vector<const char *> HardwareBufferInstancePlugin::getLayers() {
        return {};
    }

    void HardwareBufferInstancePlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {

    }

    void HardwareBufferInstancePlugin::onInstanceCreated(Instance &instance) {

    }

} // vklite
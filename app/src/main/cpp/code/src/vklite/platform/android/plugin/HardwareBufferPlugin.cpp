//
// Created by leixing on 2025/5/29.
//

#include "HardwareBufferPlugin.h"
#include "vklite/platform/android/device/HardwareBufferDevicePlugin.h"
#include "vklite/platform/android/instance/HardwareBufferInstancePlugin.h"

namespace vklite {

    HardwareBufferPlugin::HardwareBufferPlugin() = default;

    HardwareBufferPlugin::~HardwareBufferPlugin() = default;

    std::unique_ptr<DevicePluginInterface> HardwareBufferPlugin::createDevicePlugin() {
        return std::make_unique<HardwareBufferDevicePlugin>();
    }

    std::unique_ptr<InstancePluginInterface> HardwareBufferPlugin::createInstancePlugin() {
        return std::make_unique<HardwareBufferInstancePlugin>();
    }

} // vklite
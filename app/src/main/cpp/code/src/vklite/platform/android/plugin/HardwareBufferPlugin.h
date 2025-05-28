//
// Created by leixing on 2025/5/29.
//

#pragma once

#include "vklite/plugin/PluginInterface.h"

namespace vklite {

    class HardwareBufferPlugin : public PluginInterface {
    private:

    public:
        HardwareBufferPlugin();

        ~HardwareBufferPlugin() override;

        std::unique_ptr<DevicePluginInterface> createDevicePlugin() override;

        std::unique_ptr<InstancePluginInterface> createInstancePlugin() override;
    };

} // vklite

//
// Created by leixing on 2025/4/28.
//

#pragma once

#include "vklite/plugin/PluginInterface.h"

namespace vklite {

    class WindowsPlugin : public PluginInterface {
    private:
    public:

        std::vector<const char *> getInstanceExtensions() override;

        std::vector<const char *> getDeviceExtensions() override;

        std::vector<const char *> getLayers() override;

        void onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) override;
    };

} // vklite

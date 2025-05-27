//
// Created by leixing on 2025/4/28.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/device/DevicePluginInterface.h"
#include "vklite/instance/InstancePluginInterface.h"

namespace vklite {

    class PluginInterface {
    public:
        PluginInterface();

        virtual ~PluginInterface();

        virtual std::unique_ptr<DevicePluginInterface> createDevicePlugin() = 0;

        virtual std::unique_ptr<InstancePluginInterface> createInstancePlugin() = 0;
    };

} // vklite
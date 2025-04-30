//
// Created by leixing on 2025/4/28.
//

#pragma once

#include <vector>

#include "vulkan/vulkan.hpp"

namespace vklite {

    class DevicePlugin {
    public:
        DevicePlugin();

        virtual ~DevicePlugin();

        virtual std::vector<const char *> getInstanceExtensions() = 0;

        virtual std::vector<const char *> getDeviceExtensions() = 0;

        virtual std::vector<const char *> getLayers() = 0;

        virtual void onCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) = 0;
    };

} // vklite
//
// Created by leixing on 2025/4/28.
//

#pragma once

#include "PhysicalDeviceFilter.h"

namespace vklite {

    class DeviceExtensionsPhysicalDeviceFilter : public PhysicalDeviceFilter {
    private:
        std::vector<std::string> mExtensionNames;

    public:
        explicit DeviceExtensionsPhysicalDeviceFilter(std::vector<std::string> &&extensionNames);

        ~DeviceExtensionsPhysicalDeviceFilter() override;

        [[nodiscard]]
        bool test(const PhysicalDevice &physicalDevice) const override;
    };

} // vklite

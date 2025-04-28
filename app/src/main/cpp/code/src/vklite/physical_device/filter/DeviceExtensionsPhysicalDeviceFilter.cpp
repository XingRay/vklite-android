//
// Created by leixing on 2025/4/28.
//

#include "DeviceExtensionsPhysicalDeviceFilter.h"

namespace vklite {

    DeviceExtensionsPhysicalDeviceFilter::DeviceExtensionsPhysicalDeviceFilter(std::vector<std::string> &&extensionNames)
            : mExtensionNames(std::move(extensionNames)) {}

    DeviceExtensionsPhysicalDeviceFilter::~DeviceExtensionsPhysicalDeviceFilter() = default;

    bool DeviceExtensionsPhysicalDeviceFilter::test(const PhysicalDevice &physicalDevice)const {
        return physicalDevice.isSupportExtensions(mExtensionNames);
    }

} // vklite
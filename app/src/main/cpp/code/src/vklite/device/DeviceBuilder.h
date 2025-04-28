//
// Created by leixing on 2025/4/28.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vklite/physical_device/PhysicalDevice.h"
#include "vklite/device/Device.h"
#include "vklite/device/DevicePlugin.h"

namespace vklite {

    class DeviceBuilder {
    private:
        std::vector<std::string> mDeviceExtensions;
        std::vector<std::string> mLayers;
        std::vector<std::unique_ptr<DevicePlugin>> mDevicePlugins;

        std::vector<uint32_t> mGraphicQueueIndices;
        std::vector<uint32_t> mPresentQueueIndices;
        std::vector<uint32_t> mComputeQueueIndices;
    public:
        DeviceBuilder();

        ~DeviceBuilder();

        DeviceBuilder &addDevicePlugin(std::unique_ptr<DevicePlugin> devicePlugin);

        DeviceBuilder &addGraphicQueueIndex(uint32_t queueIndex);

        DeviceBuilder &addPresentQueueIndex(uint32_t queueIndex);

        DeviceBuilder &addComputeQueueIndex(uint32_t queueIndex);

        std::unique_ptr<Device> build(const PhysicalDevice &physicalDevice);
    };

} // vklite

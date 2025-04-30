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
        std::vector<const char *> mExtensions;
        std::vector<const char *> mLayers;
        std::vector<std::unique_ptr<DevicePlugin>> mDevicePlugins;

        std::vector<uint32_t> mGraphicQueueIndices;
        std::vector<uint32_t> mPresentQueueIndices;
        std::vector<uint32_t> mComputeQueueIndices;

        vk::PhysicalDeviceFeatures mPhysicalDeviceFeatures;

    public:
        DeviceBuilder();

        ~DeviceBuilder();

        DeviceBuilder &addDevicePlugin(std::unique_ptr<DevicePlugin> devicePlugin);

        DeviceBuilder &addGraphicQueueIndex(uint32_t queueIndex);

        DeviceBuilder &addPresentQueueIndex(uint32_t queueIndex);

        DeviceBuilder &addComputeQueueIndex(uint32_t queueIndex);

        DeviceBuilder &extensions(std::vector<const char *> &&extensions);

        DeviceBuilder &layers(std::vector<const char *> &&layers);

        DeviceBuilder &enableSamplerAnisotropy();

        DeviceBuilder &enableSampleRateShading();

        std::unique_ptr<Device> build(const PhysicalDevice &physicalDevice);
    };

} // vklite

//
// Created by leixing on 2025/4/28.
//

#include "DeviceBuilder.h"

namespace vklite {

    DeviceBuilder::DeviceBuilder() = default;

    DeviceBuilder::~DeviceBuilder() = default;

    DeviceBuilder &DeviceBuilder::addDevicePlugin(std::unique_ptr<DevicePlugin> devicePlugin) {
        mDevicePlugins.push_back(std::move(devicePlugin));
        return *this;
    }

    DeviceBuilder &DeviceBuilder::addGraphicQueueIndex(uint32_t queueIndex) {
        mGraphicQueueIndices.push_back(queueIndex);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::addPresentQueueIndex(uint32_t queueIndex) {
        mPresentQueueIndices.push_back(queueIndex);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::addComputeQueueIndex(uint32_t queueIndex) {
        mComputeQueueIndices.push_back(queueIndex);
        return *this;
    }

    std::unique_ptr<Device> DeviceBuilder::build(const PhysicalDevice &physicalDevice) {

        return nullptr;
    }

} // vklite
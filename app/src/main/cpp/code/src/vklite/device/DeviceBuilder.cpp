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

    DeviceBuilder &DeviceBuilder::extensions(std::vector<const char *> &&extensions) {
        mExtensions = std::move(extensions);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::layers(std::vector<const char *> &&layers) {
        mLayers = std::move(layers);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::enableSamplerAnisotropy() {
        mPhysicalDeviceFeatures.setSamplerAnisotropy(vk::True);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::enableSampleRateShading() {
        mPhysicalDeviceFeatures.setSampleRateShading(vk::True);
        return *this;
    }

    std::unique_ptr<Device> DeviceBuilder::build(const PhysicalDevice &physicalDevice) {
        std::unordered_map<vk::QueueFlagBits, std::vector<uint32_t>> queueFamilyIndicesMap;
        queueFamilyIndicesMap.try_emplace(vk::QueueFlagBits::eGraphics, std::move(mGraphicQueueIndices));
        queueFamilyIndicesMap.try_emplace(vk::QueueFlagBits::eCompute, std::move(mComputeQueueIndices));

        return std::make_unique<Device>(physicalDevice, queueFamilyIndicesMap, mPresentQueueIndices,
                                        std::move(mExtensions), std::move(mLayers),
                                        mPhysicalDeviceFeatures, std::move(mDevicePlugins));
    }

} // vklite
//
// Created by leixing on 2025/4/28.
//

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <unordered_map>
#include <functional>
#include <optional>

#include "vklite/physical_device/PhysicalDevice.h"
#include "vklite/device/Device.h"
#include "vklite/device/DevicePluginInterface.h"
#include "vklite/device/QueueFamilyConfigure.h"

namespace vklite {

    class DeviceBuilder {
    private:
        // flags
        vk::DeviceCreateFlags mFlags;


        // physcial device
        vk::PhysicalDevice mPhysicalDevice;


        // queue create info
        std::unordered_map<uint32_t, QueueFamilyConfigure> mQueueFamilyConfigures;

        // physical device properties
        vk::PhysicalDeviceFeatures mRequiredPhysicalDeviceFeatures;
        bool mCheckPhysicalDeviceFeatures;

        // extentions and layers
        std::vector<const char *> mExtensions;
        std::vector<const char *> mLayers;

        // plugins
        std::vector<std::unique_ptr<DevicePluginInterface>> mDevicePlugins;


    public:
        DeviceBuilder();

        ~DeviceBuilder();

        // flags
        DeviceBuilder &flags(vk::DeviceCreateFlags flags);

        // physicalDevice
        DeviceBuilder &physicalDevice(vk::PhysicalDevice physicalDevice);

        // addDevicePlugin
        DeviceBuilder &addDevicePlugin(std::unique_ptr<DevicePluginInterface> devicePlugin);


        // addQueueFamily
        DeviceBuilder &addQueueFamily(QueueFamilyConfigure &&queueFamilyConfigure);

        DeviceBuilder &addQueueFamily(const std::function<void(QueueFamilyConfigure &)> &configure);

        DeviceBuilder &addQueueFamily(uint32_t queueFamilyIndex);


        // extensions and layers
        DeviceBuilder &extensions(std::vector<const char *> &&extensions);

        DeviceBuilder &layers(std::vector<const char *> &&layers);


        // features
        DeviceBuilder &physicalDeviceFeaturesConfigure(const std::function<void(vk::PhysicalDeviceFeatures &physicalDeviceFeatures)> &configure);

        DeviceBuilder &enableSamplerAnisotropy();

        DeviceBuilder &enableSampleRateShading();

        DeviceBuilder &checkPhysicalDeviceFeatures(bool enable);


        // build
        std::optional<Device>

        build();

        std::unique_ptr<Device> buildUnique();

    private:
        void checkPhysicalDeviceFeatures();
    };

} // vklite

//
// Created by leixing on 2025/4/28.
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <functional>
#include <optional>

#include "vklite/device/Device.h"
#include "vklite/plugin/PluginInterface.h"

namespace vklite {

    class DeviceBuilder {
    private:
        vk::PhysicalDevice mPhysicalDevice;

        vk::DeviceCreateInfo mDeviceCreateInfo;

        std::vector<vk::DeviceQueueCreateInfo> mDeviceQueueCreateInfos;
        std::vector<std::vector<float>> mQueuePriorities;

        // physical device properties
        bool mCheckPhysicalDeviceFeatures;
        vk::PhysicalDeviceFeatures mRequiredPhysicalDeviceFeatures;

        std::vector<const char *> mExtensions;
        std::vector<const char *> mLayers;

        // plugins
        std::vector<std::unique_ptr<PluginInterface>> mPlugins;

    public:
        DeviceBuilder();

        ~DeviceBuilder();

        DeviceBuilder(const DeviceBuilder &other) = delete;

        DeviceBuilder &operator=(const DeviceBuilder &other) = delete;

        DeviceBuilder(DeviceBuilder &&other) noexcept;

        DeviceBuilder &operator=(DeviceBuilder &&other) noexcept;

        DeviceBuilder &physicalDevice(vk::PhysicalDevice physicalDevice);

        DeviceBuilder &flags(vk::DeviceCreateFlags flags);

        DeviceBuilder &addPlugin(std::unique_ptr<PluginInterface> plugin);


        // addQueueFamily
        DeviceBuilder &addQueueFamily(uint32_t queueFamilyIndex, std::vector<float> &&priorities);

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
        Device build();

        std::unique_ptr<Device> buildUnique();

    private:
        void checkPhysicalDeviceFeatures();
    };

} // vklite

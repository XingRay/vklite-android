//
// Created by leixing on 2025/5/29.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/plugin/PluginInterface.h"
#include "vklite/instance/Instance.h"

namespace vklite {

    class HardwareBufferPlugin : public PluginInterface {
    private:
        vk::PhysicalDeviceFeatures2 mPhysicalDeviceFeatures2;
        vk::PhysicalDeviceSamplerYcbcrConversionFeatures mPhysicalDeviceSamplerYcbcrConversionFeatures;

    public:
        HardwareBufferPlugin();

        ~HardwareBufferPlugin() override;

        HardwareBufferPlugin(const HardwareBufferPlugin &other) = delete;

        HardwareBufferPlugin &operator=(const HardwareBufferPlugin &other) = delete;

        HardwareBufferPlugin(HardwareBufferPlugin &&other) noexcept;

        HardwareBufferPlugin &operator=(HardwareBufferPlugin &&other) noexcept;

        std::vector<const char *> getInstanceExtensions() override;

        std::vector<const char *> getInstanceLayers() override;

        void onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) override;

        void onInstanceCreated(Instance &instance) override;


        std::vector<const char *> getDeviceExtensions() override;

        std::vector<const char *> getDeviceLayers() override;

        void physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) override;

        void onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) override;

    public: // static
        static std::unique_ptr<HardwareBufferPlugin> buildUnique();
    };

} // vklite

//
// Created by leixing on 2025/6/3.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/plugin/PluginInterface.h"
#include "vklite/instance/Instance.h"

namespace vklite {

    class CombinedPlugin : public PluginInterface {
    private:
        std::vector<std::unique_ptr<PluginInterface>> mDependencies;

    public:
        explicit CombinedPlugin();

        ~CombinedPlugin() override;

        std::vector<const char *> getInstanceExtensions() override;

        std::vector<const char *> getInstanceLayers() override;

        void onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) override;

        void onInstanceCreated(Instance &instance) override;


        std::vector<const char *> getDeviceExtensions() override;

        std::vector<const char *> getDeviceLayers() override;

        void physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) override;

        void onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) override;


        CombinedPlugin &addDependency(std::unique_ptr<PluginInterface> plugin);

    public: // static
        static std::unique_ptr<CombinedPlugin> buildUnique();

    };

} // vklite

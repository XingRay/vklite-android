//
// Created by leixing on 2025/4/27.
//

#pragma once

//#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
//#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
//#endif

#include <vulkan/vulkan.hpp>

#include "vklite/util/selector/ListSelector.h"
#include "vklite/instance/Instance.h"
#include "vklite/plugin/PluginInterface.h"

#ifndef VKLITE_APPLICATION_NAME
#define VKLITE_APPLICATION_NAME "vklite_application"
#endif

#ifndef VKLITE_APPLICATION_VERSION
#define VKLITE_APPLICATION_VERSION 1
#endif

#ifndef VKLITE_ENGINE_NAME
#define VKLITE_ENGINE_NAME "vklite_engine"
#endif

#ifndef VKLITE_ENGINE_VERSION
#define VKLITE_ENGINE_VERSION 1
#endif

namespace vklite {

    class InstanceBuilder {
    private:
        vk::InstanceCreateInfo mInstanceCreateInfo;
        vk::ApplicationInfo mApplicationInfo;

        std::vector<const char *> mExtensions;
        std::vector<const char *> mLayers;

        std::vector<std::unique_ptr<PluginInterface>> mPlugins;

    public:

        InstanceBuilder();

        ~InstanceBuilder();

        InstanceBuilder(const InstanceBuilder &other) = delete;

        InstanceBuilder &operator=(const InstanceBuilder &other) = delete;

        InstanceBuilder(InstanceBuilder &&other) noexcept;

        InstanceBuilder &operator=(InstanceBuilder &&other) noexcept;

        InstanceBuilder &applicationName(const char *applicationName);

        InstanceBuilder &applicationVersion(uint32_t applicationVersion);

        InstanceBuilder &engineName(const char *engineName);

        InstanceBuilder &engineVersion(uint32_t engineVersion);

        InstanceBuilder &extensions(std::vector<const char *> &&extensions);

        InstanceBuilder &layers(std::vector<const char *> &&layers);

        InstanceBuilder &addPlugin(std::unique_ptr<PluginInterface> plugin);

        // build
        Instance build();

        std::unique_ptr<Instance> buildUnique();
    };

} // vklite

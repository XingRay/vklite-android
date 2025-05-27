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
#include "vklite/instance/InstancePluginInterface.h"

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
        const char *mApplicationName;
        uint32_t mApplicationVersion;
        const char *mEngineName;
        uint32_t mEngineVersion;

        std::unique_ptr<ListSelector<const char *>> mExtensionsSelector;
        std::unique_ptr<ListSelector<const char *>> mLayersSelector;

        std::vector<std::unique_ptr<InstancePluginInterface>> mInstancePlugins;

    public:

        InstanceBuilder();

        ~InstanceBuilder();

        InstanceBuilder &applicationName(const char *applicationName);

        InstanceBuilder &applicationVersion(uint32_t applicationVersion);

        InstanceBuilder &engineName(const char *engineName);

        InstanceBuilder &engineVersion(uint32_t engineVersion);

        InstanceBuilder &extensionsSelector(std::unique_ptr<ListSelector<const char *>> extensionsSelector);

        InstanceBuilder &extensions(std::vector<const char *> &&required, std::vector<const char *> &&optional = {});

        InstanceBuilder &layersSelector(std::unique_ptr<ListSelector<const char *>> layersSelector);

        InstanceBuilder &layers(std::vector<const char *> &&required, std::vector<const char *> &&optional = {});

        InstanceBuilder &addInstancePlugin(std::unique_ptr<InstancePluginInterface> instancePlugin);

        // build
        Instance build();

        std::unique_ptr<Instance> buildUnique();
    };

} // vklite

//
// Created by leixing on 2025/4/27.
//

#pragma once

#include <string>

#include "vklite/util/selector/Selector.h"
#include "vklite/instance/Instance.h"

#ifndef VKLITE_APPLICATION_NAME
#define VKLITE_APPLICATION_NAME "vklite_application"
#endif

#ifndef VKLITE_ENGINE_NAME
#define VKLITE_ENGINE_NAME "vklite_engine"
#endif

namespace vklite {

    class InstanceBuilder {
    private:
        std::string mApplicationName;
        uint32_t mApplicationVersion;
        std::string mEngineName;
        uint32_t mEngineVersion;

        std::unique_ptr<ListSelector<std::string>> mExtensionsSelector;
        std::unique_ptr<ListSelector<std::string>> mLayersSelector;

    public:

        InstanceBuilder();

        ~InstanceBuilder();

        InstanceBuilder &applicationName(std::string applicationName);

        InstanceBuilder &applicationVersion(uint32_t applicationVersion);

        InstanceBuilder &engineName(std::string engineName);

        InstanceBuilder &engineVersion(uint32_t engineVersion);

        InstanceBuilder &extensionsSelector(std::unique_ptr<ListSelector<std::string>> extensionsSelector);

        InstanceBuilder &extensions(std::vector<std::string> &&required, std::vector<std::string> &&optional = {});

        InstanceBuilder &layersSelector(std::unique_ptr<ListSelector<std::string>> layersSelector);

        InstanceBuilder &layers(std::vector<std::string> &&required, std::vector<std::string> &&optional = {});

        std::unique_ptr<Instance> build();
    };

} // vklite

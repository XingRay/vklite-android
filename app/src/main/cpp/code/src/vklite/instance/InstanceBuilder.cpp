//
// Created by leixing on 2025/4/27.
//

#include "InstanceBuilder.h"

#include "vklite/util/selector/StringListSelector.h"

namespace vklite {

    InstanceBuilder::InstanceBuilder()
            : mApplicationName(VKLITE_APPLICATION_NAME), mEngineName(VKLITE_ENGINE_NAME) {}

    InstanceBuilder::~InstanceBuilder() = default;

    InstanceBuilder &InstanceBuilder::applicationName(std::string applicationName) {
        mApplicationName = std::move(applicationName);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::applicationVersion(uint32_t applicationVersion) {
        mApplicationVersion = applicationVersion;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::engineName(std::string engineName) {
        mEngineName = std::move(engineName);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::engineVersion(uint32_t engineVersion) {
        mEngineVersion = engineVersion;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::extensionsSelector(std::unique_ptr<ListSelector<std::string>> extensionsSelector) {
        mExtensionsSelector = std::move(extensionsSelector);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::extensions(std::vector<std::string> &&required, std::vector<std::string> &&optional) {
        mExtensionsSelector = std::make_unique<RequiredAndOptionalStringListSelector>(std::move(required), std::move(optional));
        return *this;
    }

    InstanceBuilder &InstanceBuilder::layersSelector(std::unique_ptr<ListSelector<std::string>> layersSelector) {
        mLayersSelector = std::move(layersSelector);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::layers(std::vector<std::string> &&required, std::vector<std::string> &&optional) {
        mLayersSelector = std::make_unique<RequiredAndOptionalStringListSelector>(std::move(required), std::move(optional));
        return *this;
    }

    std::unique_ptr<Instance> InstanceBuilder::build() {
        return std::make_unique<Instance>(mApplicationName, mApplicationVersion,
                                          mEngineName, mEngineVersion,
                                          *mExtensionsSelector,
                                          *mLayersSelector);
    }

} // vklite
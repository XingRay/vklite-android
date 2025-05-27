//
// Created by leixing on 2025/3/15.
//

#include "AndroidInstancePlugin.h"
#include "VulkanAndroidApi.h"

namespace vklite {

    AndroidInstancePlugin::AndroidInstancePlugin() = default;

    AndroidInstancePlugin::~AndroidInstancePlugin() = default;

    std::vector<const char *> AndroidInstancePlugin::getExtensions() {
        return {};
    }

    std::vector<const char *> AndroidInstancePlugin::getLayers() {
        return {};
    }

    void AndroidInstancePlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {

    }

    void AndroidInstancePlugin::onInstanceCreated(Instance &instance) {
        VulkanAndroidApi::initApi(instance);
    }

} // vklite
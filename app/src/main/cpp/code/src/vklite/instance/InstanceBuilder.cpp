//
// Created by leixing on 2025/4/27.
//

#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#include "InstanceBuilder.h"

#include "vklite/util/selector/cstring/CStringFixListSelector.h"
#include "vklite/util/selector/cstring/CStringLambdaListSelector.h"
#include "vklite/util/selector/cstring/CStringRequiredAndOptionalListSelector.h"
#include "vklite/util/VkCheck.h"
#include "vklite/util/VkCheckCpp.h"
#include "vklite/Log.h"
#include "vklite/util/StringUtil.h"
#include "vklite/instance/InstanceApi.h"

namespace vklite {

    InstanceBuilder::InstanceBuilder() {

        mApplicationInfo = vk::ApplicationInfo{};
        mApplicationInfo
                .setPApplicationName(VKLITE_APPLICATION_NAME)
                .setApiVersion(VKLITE_APPLICATION_VERSION)
                .setPEngineName(VKLITE_ENGINE_NAME)
                .setEngineVersion(VKLITE_ENGINE_VERSION)
                .setApiVersion(VK_API_VERSION_1_3);

        mInstanceCreateInfo = vk::InstanceCreateInfo{};
        mInstanceCreateInfo
                .setPApplicationInfo(&mApplicationInfo)
                .setPEnabledLayerNames(mLayers)
                .setPEnabledExtensionNames(mExtensions);
    }

    InstanceBuilder::~InstanceBuilder() = default;

    InstanceBuilder &InstanceBuilder::applicationName(const char *applicationName) {
        mApplicationInfo.setPApplicationName(applicationName);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::applicationVersion(uint32_t applicationVersion) {
        mApplicationInfo.setApplicationVersion(applicationVersion);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::engineName(const char *engineName) {
        mApplicationInfo.setPEngineName(engineName);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::engineVersion(uint32_t engineVersion) {
        mApplicationInfo.setEngineVersion(engineVersion);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::extensions(std::vector<const char *> &&extensions) {
        mExtensions = std::move(extensions);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::layers(std::vector<const char *> &&layers) {
        mLayers = std::move(layers);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::addPlugin(std::unique_ptr<PluginInterface> plugin) {
        mPlugins.push_back(std::move(plugin));
        return *this;
    }

    Instance InstanceBuilder::build() {
        LOG_D("Instance::Instance");
        vk::DynamicLoader dynamicLoader;
        InstanceApi::initInstanceApi(dynamicLoader);

        // InstanceExtension
        std::vector<const char *> availableExtensionNames = InstanceApi::enumerateInstanceExtensionNames();
        LOG_D("Available instance extensions:[%ld]", availableExtensionNames.size());
        for (const auto &name: availableExtensionNames) {
            LOG_D("  %s", name);
        }

        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            std::vector<const char *> pluginExtensions = plugin->getInstanceExtensions();
            mExtensions.insert(mExtensions.begin(), std::move_iterator(pluginExtensions.begin()), std::move_iterator(pluginExtensions.end()));
        }
        LOG_D("enabled instance extensions:[%ld]", mExtensions.size());
        for (const auto &extensionName: mExtensions) {
            LOG_D("  %s", extensionName);
        }
        mInstanceCreateInfo.setPEnabledExtensionNames(mExtensions);

        // Available Layers
        std::vector<const char *> availableLayerNames = InstanceApi::enumerateInstanceLayerNames();
        LOG_D("Available Layers : [%ld]", availableLayerNames.size());
        for (const auto &name: availableLayerNames) {
            LOG_D("  %s", name);
        }

        // plugin->getInstanceLayers
        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            std::vector<const char *> pluginLayers = plugin->getInstanceLayers();
            mLayers.insert(mLayers.begin(), std::move_iterator(pluginLayers.begin()), std::move_iterator(pluginLayers.end()));
        }
        LOG_D("enabled layer names:[%ld]", mLayers.size());
        for (const auto &layerName: mLayers) {
            LOG_D("  %s", layerName);
        }
        mInstanceCreateInfo.setPEnabledLayerNames(mLayers);


        // plugin->onPreCreateInstance
        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            plugin->onPreCreateInstance((vk::InstanceCreateInfo &) mInstanceCreateInfo);
        }

        vk::Instance vkInstance = InstanceApi::createInstance(mInstanceCreateInfo);

        vk::DispatchLoaderDynamic dispatchLoaderDynamic(vkInstance, vkGetInstanceProcAddr);
        dispatchLoaderDynamic.init(dynamicLoader);
        vk::defaultDispatchLoaderDynamic = dispatchLoaderDynamic;

        Instance instance(vkInstance);

        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            plugin->onInstanceCreated(instance);
        }

        return instance;
    }

    std::unique_ptr<Instance> InstanceBuilder::buildUnique() {
        return std::make_unique<Instance>(build());
    }

} // vklite
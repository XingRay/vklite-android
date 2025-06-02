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

    InstanceBuilder::InstanceBuilder()
            : mApplicationName(VKLITE_APPLICATION_NAME),
              mApplicationVersion(VKLITE_APPLICATION_VERSION),
              mEngineName(VKLITE_ENGINE_NAME),
              mEngineVersion(VKLITE_ENGINE_VERSION) {}

    InstanceBuilder::~InstanceBuilder() = default;

    InstanceBuilder &InstanceBuilder::applicationName(const char *applicationName) {
        mApplicationName = applicationName;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::applicationVersion(uint32_t applicationVersion) {
        mApplicationVersion = applicationVersion;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::engineName(const char *engineName) {
        mEngineName = engineName;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::engineVersion(uint32_t engineVersion) {
        mEngineVersion = engineVersion;
        return *this;
    }

    InstanceBuilder &InstanceBuilder::extensionsSelector(std::unique_ptr<ListSelector<const char *>> extensionsSelector) {
        mExtensionsSelector = std::move(extensionsSelector);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::extensions(std::vector<const char *> &&required, std::vector<const char *> &&optional) {
        mExtensionsSelector = std::make_unique<CStringRequiredAndOptionalListSelector>(std::move(required), std::move(optional));
        return *this;
    }

    InstanceBuilder &InstanceBuilder::layersSelector(std::unique_ptr<ListSelector<const char *>> layersSelector) {
        mLayersSelector = std::move(layersSelector);
        return *this;
    }

    InstanceBuilder &InstanceBuilder::layers(std::vector<const char *> &&required, std::vector<const char *> &&optional) {
        mLayersSelector = std::make_unique<CStringRequiredAndOptionalListSelector>(std::move(required), std::move(optional));
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
        uint32_t extensionCount = 0;
        CALL_VK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr));
        std::vector<VkExtensionProperties> availableInstanceExtensions(extensionCount);
        LOG_D("Available instance extensions:[%d]", extensionCount);

        CALL_VK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableInstanceExtensions.data()));

        std::vector<const char *> availableExtensionNames;
        for (const auto &extensionProperties: availableInstanceExtensions) {
            LOG_D("  %s", extensionProperties.extensionName);
            availableExtensionNames.push_back(extensionProperties.extensionName);
        }

        std::vector<const char *> enabledInstanceExtensionNames = mExtensionsSelector->select(availableExtensionNames);
        LOG_D("enabled instance extensions:[%ld]", enabledInstanceExtensionNames.size());
        for (const auto &extensionName: enabledInstanceExtensionNames) {
            LOG_D("  %s", extensionName);
        }


        // Layer
        uint32_t layerCount = 0;
        CALL_VK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));
        std::vector<VkLayerProperties> availableLayers(layerCount);
        LOG_D("available layers:[%d]", layerCount);

        CALL_VK(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()));

        std::vector<const char *> availableLayerNames;
        for (const auto &layerProperties: availableLayers) {
            LOG_D("  %s", layerProperties.layerName);
            availableLayerNames.push_back(layerProperties.layerName);
        }

        std::vector<const char *> enabledLayerNames = mLayersSelector->select(availableLayerNames);
        LOG_D("enabled layer names:[%ld]", enabledLayerNames.size());
        for (const auto &layerName: enabledLayerNames) {
            LOG_D("  %s", layerName);
        }


        VkApplicationInfo appInfo = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext = nullptr,
                .pApplicationName = mApplicationName,
                .applicationVersion = mApplicationVersion,
                .pEngineName = mEngineName,
                .engineVersion = mEngineVersion,
                .apiVersion = VK_API_VERSION_1_3,
        };

        VkInstanceCreateInfo instanceCreateInfo{
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pNext = nullptr,
                .pApplicationInfo = &appInfo,
                .enabledLayerCount = static_cast<uint32_t>(enabledLayerNames.size()),
                .ppEnabledLayerNames = enabledLayerNames.data(),
                .enabledExtensionCount = static_cast<uint32_t>(enabledInstanceExtensionNames.size()),
                .ppEnabledExtensionNames = enabledInstanceExtensionNames.data(),
        };

//        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
//        if (mEnableValidationLayer) {
//            if (!checkValidationLayerSupported()) {
//                throw std::runtime_error("validation layers required, but not available !");
//            }
//            createInfo.setPEnabledLayerNames(mValidationLayers);

//        populateDebugMessengerCreateInfo(debugCreateInfo);
//        instanceCreateInfo.pNext = &debugCreateInfo;
//        } else {
//            createInfo.enabledLayerCount = 0;
//            createInfo.pNext = nullptr;
//        }

        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            plugin->onPreCreateInstance((vk::InstanceCreateInfo &) instanceCreateInfo);
        }

        vk::Instance vkInstance;
        CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, (VkInstance *) &vkInstance));

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
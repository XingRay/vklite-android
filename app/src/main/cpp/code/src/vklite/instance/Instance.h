//
// Created by leixing on 2024/12/22.
//

#pragma once

#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#include <string>

#include <vulkan/vulkan.hpp>

#include "vklite/util/selector/ListSelector.h"
#include "vklite/physical_device/PhysicalDevice.h"

namespace vklite {

    class Instance {
    private:
        vk::Instance mInstance;
        vk::DebugReportCallbackEXT mDebugReportCallback;
        std::vector<std::string> mEnabledInstanceExtensionNames;
        std::vector<std::string> mEnabledLayerNames;

        vk::DebugUtilsMessengerEXT mDebugMessenger;

    public:
        Instance(const std::string &applicationName,
                 uint32_t applicationVersion,
                 const std::string &engineName,
                 uint32_t engineVersion,
                 const ListSelector<std::string> &extensionsSelector,
                 const ListSelector<std::string> &layersSelector);

        ~Instance();

        [[nodiscard]]
        const vk::Instance &getInstance() const;

        [[nodiscard]]
        const std::vector<std::string> &getEnabledExtensions() const;

        [[nodiscard]]
        const std::vector<std::string> &getEnabledLayers() const;

        [[nodiscard]]
        std::vector<PhysicalDevice> listPhysicalDevices() const;

    private:
        void setupDebugCallback();

        void setupDebugMessenger();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackImpl(
                VkDebugReportFlagsEXT flags,
                VkDebugReportObjectTypeEXT objectType,
                uint64_t object,
                size_t location,
                int32_t messageCode,
                const char *pLayerPrefix,
                const char *pMessage,
                void *pUserData);

        void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo);

        vk::Result CreateDebugUtilsMessengerEXT(vk::Instance instance,
                                                const vk::DebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                const vk::AllocationCallbacks *pAllocator,
                                                vk::DebugUtilsMessengerEXT *pDebugMessenger);
    };
}

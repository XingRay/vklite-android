//
// Created by leixing on 2024/12/22.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class Instance {
    private:
        vk::Instance mInstance;
//        vk::DebugReportCallbackEXT mDebugReportCallback;
//        std::vector<std::string> mEnabledInstanceExtensionNames;
//        std::vector<std::string> mEnabledLayerNames;
//
//        vk::DebugUtilsMessengerEXT mDebugMessenger;

    public:
        explicit Instance(const vk::Instance &instance);

        ~Instance();

        Instance(const Instance &other) = delete;

        Instance &operator=(const Instance &other) = delete;

        Instance(Instance &&other) noexcept;

        Instance &operator=(Instance &&other) noexcept;

        [[nodiscard]]
        const vk::Instance &getVkInstance() const;

        [[nodiscard]]
        const std::vector<std::string> &getEnabledExtensions() const;

        [[nodiscard]]
        const std::vector<std::string> &getEnabledLayers() const;

        [[nodiscard]]
        std::vector<vk::PhysicalDevice> enumeratePhysicalDevices() const;

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

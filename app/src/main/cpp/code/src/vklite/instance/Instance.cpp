//
// Created by leixing on 2024/12/22.
//

#include "Instance.h"

#include <utility>

#include "vklite/util/VkCheck.h"
#include "vklite/util/VkCheckCpp.h"
#include "vklite/Log.h"
#include "vklite/util/StringUtil.h"

namespace vklite {

//    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
//            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
//            VkDebugUtilsMessageTypeFlagsEXT messageType,
//            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
//            void *pUserData);

    Instance::Instance(const vk::Instance& instance)
            : mInstance(instance) {}

    Instance::~Instance() {
        if (mInstance != nullptr) {
            mInstance.destroy();
            mInstance = nullptr;
        }
    }

    Instance::Instance(Instance &&other) noexcept
            : mInstance(std::exchange(other.mInstance, nullptr)) {}

    Instance &Instance::operator=(Instance &&other) noexcept {
        if (this != &other) {
            mInstance = std::exchange(other.mInstance, nullptr);
        }
        return *this;
    }

    const vk::Instance &Instance::getVkInstance() const {
        return mInstance;
    }

    std::vector<vk::PhysicalDevice> Instance::enumeratePhysicalDevices() const {
        return mInstance.enumeratePhysicalDevices();
    }

    void Instance::setupDebugCallback() {
//        vk::DebugReportCallbackCreateInfoEXT debugInfo{
//                vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning,
//                debugCallbackImpl,
//                nullptr
//        };
//
//        mDebugReportCallback = mInstance.createDebugReportCallbackEXT(debugInfo);
    }


//    VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debugCallbackImpl(
//            VkDebugReportFlagsEXT flags,
//            VkDebugReportObjectTypeEXT objectType,
//            uint64_t object,
//            size_t location,
//            int32_t messageCode,
//            const char *pLayerPrefix,
//            const char *pMessage,
//            void *pUserData) {
//        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
//            LOG_E("[ERROR] %s: %s", pLayerPrefix, pMessage);
//        } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
//            LOG_E("[WARNING] %s: %s", pLayerPrefix, pMessage);
//        } else {
//            LOG_E("[INFO] %s: %s", pLayerPrefix, pMessage);
//        }
//        return VK_FALSE;
//    }


//    void Instance::populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo) {
//        createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
//                                     | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
//                                     | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
//
//        createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
//                                 | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
//                                 | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
//                                 | vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding;
//
//        createInfo.pfnUserCallback = debugCallback;
//    }

//    void Instance::setupDebugMessenger() {
//        LOG_D("setupDebugMessenger");
////        if (!mEnableValidationLayer) {
////            return;
////        }
//
//        vk::DebugUtilsMessengerCreateInfoEXT createInfo;
//        populateDebugMessengerCreateInfo(createInfo);
//
//        if (CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger) != vk::Result::eSuccess) {
//            throw std::runtime_error("failed to set up debug messenger!");
//        }
//    }


//    vk::Result Instance::CreateDebugUtilsMessengerEXT(vk::Instance instance,
//                                                      const vk::DebugUtilsMessengerCreateInfoEXT *pCreateInfo,
//                                                      const vk::AllocationCallbacks *pAllocator,
//                                                      vk::DebugUtilsMessengerEXT *pDebugMessenger) {
//        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
//        if (func == nullptr) {
//            return vk::Result::eErrorExtensionNotPresent;
//        }
//
//        VkResult vkResult = func(instance, (VkDebugUtilsMessengerCreateInfoEXT *) pCreateInfo, (VkAllocationCallbacks *) pAllocator, (VkDebugUtilsMessengerEXT *) pDebugMessenger);
//        if (vkResult != VK_SUCCESS) {
//            throw std::runtime_error("vkCreateDebugUtilsMessengerEXT failed !");
//        }
//        return vk::Result::eSuccess;
//    }

//    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
//            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
//            VkDebugUtilsMessageTypeFlagsEXT messageType,
//            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
//            void *pUserData) {
//
//        LOG_E("validation layer: %s", pCallbackData->pMessage);
//
//        return VK_FALSE;
//    }
}

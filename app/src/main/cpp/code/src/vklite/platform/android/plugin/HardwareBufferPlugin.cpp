//
// Created by leixing on 2025/5/29.
//

#include "HardwareBufferPlugin.h"

namespace vklite {

    HardwareBufferPlugin::HardwareBufferPlugin() {
        mPhysicalDeviceSamplerYcbcrConversionFeatures.samplerYcbcrConversion = true;
        mPhysicalDeviceFeatures2.features.samplerAnisotropy = true;

        mPhysicalDeviceFeatures2.pNext = &mPhysicalDeviceSamplerYcbcrConversionFeatures;
    }

    HardwareBufferPlugin::~HardwareBufferPlugin() = default;

    HardwareBufferPlugin::HardwareBufferPlugin(HardwareBufferPlugin &&other) noexcept
            : mPhysicalDeviceFeatures2(other.mPhysicalDeviceFeatures2),
              mPhysicalDeviceSamplerYcbcrConversionFeatures(other.mPhysicalDeviceSamplerYcbcrConversionFeatures) {
        mPhysicalDeviceFeatures2.pNext = &mPhysicalDeviceSamplerYcbcrConversionFeatures;
    }

    HardwareBufferPlugin &HardwareBufferPlugin::operator=(HardwareBufferPlugin &&other) noexcept {
        if (this != &other) {
            mPhysicalDeviceFeatures2 = other.mPhysicalDeviceFeatures2;
            mPhysicalDeviceSamplerYcbcrConversionFeatures = other.mPhysicalDeviceSamplerYcbcrConversionFeatures;

            mPhysicalDeviceFeatures2.pNext = &mPhysicalDeviceSamplerYcbcrConversionFeatures;
        }
        return *this;
    }

    std::vector<const char *> HardwareBufferPlugin::getInstanceExtensions() {
        return {
                VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME,
                VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
        };
    }

    std::vector<const char *> HardwareBufferPlugin::getInstanceLayers() {
        return {

        };
    }

    void HardwareBufferPlugin::onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) {

    }

    void HardwareBufferPlugin::onInstanceCreated(Instance &instance) {

    }


    std::vector<const char *> HardwareBufferPlugin::getDeviceExtensions() {
        return {
                VK_KHR_MAINTENANCE1_EXTENSION_NAME,
                VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
                VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
                VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME,
                VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME,
                VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME,
                VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
                VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
        };
    }

    std::vector<const char *> HardwareBufferPlugin::getDeviceLayers() {
        return {

        };
    }

    void HardwareBufferPlugin::physicalDeviceFeaturesConfigure(vk::PhysicalDeviceFeatures &physicalDeviceFeatures) {

    }

    void HardwareBufferPlugin::onPreCreateDevice(vk::DeviceCreateInfo &deviceCreateInfo) {
        deviceCreateInfo.setPNext(&mPhysicalDeviceFeatures2);
    }

    /*
     *
     * static methods
     *
     */
    std::unique_ptr<HardwareBufferPlugin> HardwareBufferPlugin::buildUnique() {
        std::unique_ptr<HardwareBufferPlugin> plugin = std::make_unique<HardwareBufferPlugin>();

        return std::move(plugin);
    }

} // vklite
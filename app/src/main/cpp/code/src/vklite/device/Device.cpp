//
// Created by leixing on 2024/12/23.
//

#include <map>

#include "Device.h"
#include "vklite/Log.h"
#include "vklite/util/VulkanUtil.h"

#include "vklite/util/StringUtil.h"
#include <unordered_set>

namespace vklite {

    Device::Device(const PhysicalDevice &physicalDevice,
                   std::unordered_map<vk::QueueFlagBits, std::vector<uint32_t>> &queueFamilyIndicesMap,
                   std::vector<uint32_t> &presentQueueFamilyIndices,
                   std::vector<const char *> &&extensions,
                   std::vector<const char *> &&layers,
                   const vk::PhysicalDeviceFeatures &physicalDeviceFeatures,
                   const std::vector<std::unique_ptr<PluginInterface>> &devicePlugins)
            : mPhysicalDevice(physicalDevice),
              mQueueFamilyIndicesMap(std::move(queueFamilyIndicesMap)),
              mPresentQueueFamilyIndices(std::move(presentQueueFamilyIndices)) {

        std::unordered_set<uint32_t> uniqueIndices;
        for (const auto &[_, indices]: mQueueFamilyIndicesMap) {
            uniqueIndices.insert(indices.begin(), indices.end());
        }
        uniqueIndices.insert(mPresentQueueFamilyIndices.begin(), mPresentQueueFamilyIndices.end());

        mQueueFamilyIndices.assign(uniqueIndices.begin(), uniqueIndices.end());

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::array<float, 1> queuePriorities = {1.0f};

        const auto &queueFamilies = mPhysicalDevice.getPhysicalDevice().getQueueFamilyProperties();

        for (uint32_t queueFamilyIndex: mQueueFamilyIndices) {
            // 验证队列家族索引合法性
            if (queueFamilyIndex >= queueFamilies.size()) {
                throw std::runtime_error("Invalid queue family index: " + std::to_string(queueFamilyIndex));
            }

            // 验证队列家族支持的最小队列数
            const uint32_t maxQueues = queueFamilies[queueFamilyIndex].queueCount;
            if (maxQueues < 1) {
                throw std::runtime_error("Queue family " + std::to_string(queueFamilyIndex) + " does not support any queues.");
            }

            vk::DeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo
                    .setQueueFamilyIndex(queueFamilyIndex)
                    .setQueueCount(1)
                    .setQueuePriorities(queuePriorities);
            queueCreateInfos.push_back(queueCreateInfo);
        }

        vk::PhysicalDeviceFeatures supportedFeatures = mPhysicalDevice.getPhysicalDevice().getFeatures();
        vk::PhysicalDeviceFeatures deviceFeatures{};

        if (physicalDeviceFeatures.samplerAnisotropy && supportedFeatures.samplerAnisotropy) {
            deviceFeatures.setSamplerAnisotropy(vk::True);
        }
        if (physicalDeviceFeatures.sampleRateShading && supportedFeatures.sampleRateShading) {
            deviceFeatures.setSampleRateShading(vk::True);
        }

        vk::DeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo
                .setFlags(vk::DeviceCreateFlags{})
                .setQueueCreateInfos(queueCreateInfos)
                .setPEnabledFeatures(&deviceFeatures);

        std::vector<const char *> enabledExtensions = std::move(extensions);
        std::vector<const char *> enabledLayers = std::move(layers);

        for (const std::unique_ptr<PluginInterface> &devicePlugin: devicePlugins) {
            devicePlugin->onCreateDevice(deviceCreateInfo);
            std::vector<const char *> pluginDeviceExtensions = devicePlugin->getDeviceExtensions();
            enabledExtensions.insert(enabledExtensions.end(), pluginDeviceExtensions.begin(), pluginDeviceExtensions.end());

            std::vector<const char *> pluginLayers = devicePlugin->getLayers();
            enabledLayers.insert(enabledLayers.end(), pluginLayers.begin(), pluginLayers.end());
        }

        deviceCreateInfo
                .setPEnabledExtensionNames(enabledExtensions)
                .setPEnabledLayerNames(enabledLayers);


        mDevice = mPhysicalDevice.getPhysicalDevice().createDevice(deviceCreateInfo);

        if (mQueueFamilyIndicesMap.contains(vk::QueueFlagBits::eGraphics)) {
            const std::vector<uint32_t> &graphicQueueFamilyIndices = mQueueFamilyIndicesMap[vk::QueueFlagBits::eGraphics];
            if (graphicQueueFamilyIndices.empty()) {
                mGraphicQueueFamilyIndex = -1;
                mGraphicsQueue = nullptr;
            } else {
                mGraphicQueueFamilyIndex = graphicQueueFamilyIndices[0];
                mGraphicsQueue = mDevice.getQueue(mGraphicQueueFamilyIndex, 0);
            }
        } else {
            mGraphicQueueFamilyIndex = -1;
            mGraphicsQueue = nullptr;
        }

        if (mPresentQueueFamilyIndices.empty()) {
            mPresentQueueFamilyIndex = -1;
            mPresentQueue = nullptr;
        } else {
            mPresentQueueFamilyIndex = mPresentQueueFamilyIndices[0];
            mPresentQueue = mDevice.getQueue(mPresentQueueFamilyIndex, 0);
        }

        if (mQueueFamilyIndicesMap.contains(vk::QueueFlagBits::eCompute)) {
            const std::vector<uint32_t> &computeQueueFamilyIndices = mQueueFamilyIndicesMap[vk::QueueFlagBits::eCompute];
            if (computeQueueFamilyIndices.empty()) {
                mComputeQueueFamilyIndex = -1;
                mComputeQueue = nullptr;
            } else {
                mComputeQueueFamilyIndex = computeQueueFamilyIndices[0];
                mComputeQueue = mDevice.getQueue(mComputeQueueFamilyIndex, 0);
            }
        } else {
            mComputeQueueFamilyIndex = -1;
            mComputeQueue = nullptr;
        }
    }

    Device::~Device() {
        LOG_D("Device::~Device");
        if (mDevice != nullptr) {
            mDevice.destroy();
        } else {
            LOG_W("mDevice is null");
        }
    }

    const PhysicalDevice &Device::getPhysicalDevice() const {
        return mPhysicalDevice;
    }

    const vk::Device &Device::getDevice() const {
        return mDevice;
    }

    uint32_t Device::getGraphicQueueFamilyIndex() const {
        return mGraphicQueueFamilyIndex;
    }

    uint32_t Device::getPresentQueueFamilyIndex() const {
        return mPresentQueueFamilyIndex;
    }

    const std::vector<uint32_t> &Device::getQueueFamilyIndices() const {
        return mQueueFamilyIndices;
    }

    vk::Queue Device::getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) const {
        return mDevice.getQueue(queueFamilyIndex, queueIndex);
    }

    const vk::Queue &Device::getGraphicsQueue() const {
        return mGraphicsQueue;
    }

    const vk::Queue &Device::getPresentQueue() const {
        return mPresentQueue;
    }

} // vklite
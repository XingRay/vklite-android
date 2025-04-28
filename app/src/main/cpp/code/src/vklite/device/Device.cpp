//
// Created by leixing on 2024/12/23.
//

#include <map>

#include "Device.h"
#include "vklite/Log.h"
#include "vklite/util/VulkanUtil.h"

#include "vklite/util/StringUtil.h"

namespace vklite {

    Device::Device(const PhysicalDevice &physicalDevice,
                   const PhysicalDeviceSurfaceSupport &surfaceSupport,
                   const std::vector<std::string> &deviceExtensions,
                   const std::vector<std::string> &layers)
            : mPhysicalDevice(physicalDevice) {

        mGraphicQueueFamilyIndex = surfaceSupport.graphicQueueFamilyIndex;
        mPresentQueueFamilyIndex = surfaceSupport.presentQueueFamilyIndex;
        LOG_D("graphicFamilyIndex:%d, presentFamilyIndex:%d", mGraphicQueueFamilyIndex, mPresentQueueFamilyIndex);

        mQueueFamilyIndices.push_back(mGraphicQueueFamilyIndex);
        if (mGraphicQueueFamilyIndex != mPresentQueueFamilyIndex) {
            mQueueFamilyIndices.push_back(mPresentQueueFamilyIndex);
        }

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::array<float, 1> queuePriorities = {1.0f};
        for (uint32_t queueFamilyIndex: mQueueFamilyIndices) {
            vk::DeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo
                    .setQueueFamilyIndex(queueFamilyIndex)
                    .setQueueCount(1)
                    .setQueuePriorities(queuePriorities);
            queueCreateInfos.push_back(queueCreateInfo);
        }

        vk::PhysicalDeviceFeatures supportedFeatures = mPhysicalDevice.getPhysicalDevice().getFeatures();
        vk::PhysicalDeviceFeatures deviceFeatures{};

        if (supportedFeatures.samplerAnisotropy) {
            deviceFeatures.setSamplerAnisotropy(vk::True);
        }
        if (supportedFeatures.sampleRateShading) {
            deviceFeatures.setSampleRateShading(vk::True);
        }

        std::vector<const char *> enabledDeviceExtensionNames = StringUtil::toStringPtrArray(deviceExtensions);
        LOG_D("enabled device extension names:[%ld]", enabledDeviceExtensionNames.size());
        for (const char *name: enabledDeviceExtensionNames) {
            LOG_D("    %s", name);
        }

        std::vector<const char *> layerNames = StringUtil::toStringPtrArray(layers);

        vk::PhysicalDeviceFeatures2 physicalDeviceFeatures2;
        vk::PhysicalDeviceSamplerYcbcrConversionFeatures ycbcr_features;
        physicalDeviceFeatures2.pNext = &ycbcr_features;

        ycbcr_features.samplerYcbcrConversion = true;
        physicalDeviceFeatures2.features.samplerAnisotropy = true;

        vk::DeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo
                .setFlags(vk::DeviceCreateFlags{})
                .setQueueCreateInfos(queueCreateInfos)
                .setPEnabledFeatures(&deviceFeatures)
                .setPEnabledExtensionNames(enabledDeviceExtensionNames)
                .setPEnabledLayerNames(layerNames)
                .setPNext(&physicalDeviceFeatures2);

        mDevice = mPhysicalDevice.getPhysicalDevice().createDevice(deviceCreateInfo);

        mGraphicsQueue = mDevice.getQueue(mGraphicQueueFamilyIndex, 0);
        mPresentQueue = mDevice.getQueue(mPresentQueueFamilyIndex, 0);

        mCapabilities = surfaceSupport.capabilities;
        mFormats = surfaceSupport.formats;
        mPresentModes = surfaceSupport.presentModes;
    }

    Device::~Device() {
        LOG_D("VulkanDevice::~VulkanDevice");
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

    const vk::Queue &Device::getGraphicsQueue() const {
        return mGraphicsQueue;
    }

    const vk::Queue &Device::getPresentQueue() const {
        return mPresentQueue;
    }

    vk::SurfaceCapabilitiesKHR Device::getCapabilities() const {
        return mCapabilities;
    }

    std::vector<vk::SurfaceFormatKHR> Device::getFormats() const {
        return mFormats;
    }

    std::vector<vk::PresentModeKHR> Device::getPresentModes() const {
        return mPresentModes;
    }

    std::pair<vk::Buffer, vk::DeviceMemory> Device::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) const {

        vk::BufferCreateInfo bufferCreateInfo{};
        bufferCreateInfo.setSize(size)
                .setUsage(usage)
                .setSharingMode(vk::SharingMode::eExclusive);

        vk::Buffer buffer = mDevice.createBuffer(bufferCreateInfo);
        vk::MemoryRequirements memoryRequirements = mDevice.getBufferMemoryRequirements(buffer);
        vk::PhysicalDeviceMemoryProperties memoryProperties = mPhysicalDevice.getPhysicalDevice().getMemoryProperties();

        uint32_t memoryType = mPhysicalDevice.findMemoryType(memoryRequirements.memoryTypeBits, properties);
        vk::MemoryAllocateInfo memoryAllocateInfo{};
        memoryAllocateInfo
                .setAllocationSize(memoryRequirements.size)
                .setMemoryTypeIndex(memoryType);

        vk::DeviceMemory bufferMemory = mDevice.allocateMemory(memoryAllocateInfo);
        mDevice.bindBufferMemory(buffer, bufferMemory, 0);

        return {buffer, bufferMemory};
    }

    std::pair<vk::Image, vk::DeviceMemory> Device::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::SampleCountFlagBits numSamples, vk::Format format,
                                                               vk::ImageTiling imageTiling, vk::ImageUsageFlags imageUsage, vk::MemoryPropertyFlags memoryProperty) const {

        vk::Extent3D extent;
        extent.setWidth(width)
                .setHeight(height)
                .setDepth(1);

        vk::ImageCreateInfo imageCreateInfo;
        imageCreateInfo.setImageType(vk::ImageType::e2D)
                .setExtent(extent)
                .setMipLevels(mipLevels)
                .setArrayLayers(1)
                .setFormat(format)
                .setTiling(imageTiling)
                .setInitialLayout(vk::ImageLayout::eUndefined)
                .setUsage(imageUsage)
                .setSharingMode(vk::SharingMode::eExclusive)
                .setSamples(numSamples)
                .setFlags(vk::ImageCreateFlags{});

        vk::Image image = mDevice.createImage(imageCreateInfo);

        vk::MemoryRequirements memoryRequirements = mDevice.getImageMemoryRequirements(image);

        vk::MemoryAllocateInfo memoryAllocateInfo;

        uint32_t memoryType = mPhysicalDevice.findMemoryType(memoryRequirements.memoryTypeBits, memoryProperty);
        memoryAllocateInfo
                .setAllocationSize(memoryRequirements.size)
                .setMemoryTypeIndex(memoryType);

        vk::DeviceMemory imageMemory = mDevice.allocateMemory(memoryAllocateInfo);

        mDevice.bindImageMemory(image, imageMemory, 0);

        return {image, imageMemory};
    }

    vk::ImageView Device::createImageView(const vk::Image &image, vk::Format format, vk::ImageAspectFlags imageAspect, uint32_t mipLevels) const {
        vk::ImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.setImage(image)
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(format);
//            .setSubresourceRange(imageSubresourceRange)
//            .setComponents(componentMapping);

        vk::ImageSubresourceRange &imageSubresourceRange = imageViewCreateInfo.subresourceRange;
        imageSubresourceRange.setAspectMask(imageAspect)
                .setBaseMipLevel(0)
                .setLevelCount(mipLevels)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        vk::ComponentMapping &componentMapping = imageViewCreateInfo.components;
        componentMapping.setR(vk::ComponentSwizzle::eIdentity)
                .setG(vk::ComponentSwizzle::eIdentity)
                .setB(vk::ComponentSwizzle::eIdentity)
                .setA(vk::ComponentSwizzle::eIdentity);

        return mDevice.createImageView(imageViewCreateInfo);
    }

} // vklite
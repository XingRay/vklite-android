//
// Created by leixing on 2025/1/19.
//
#include "HardwareBufferImage.h"

#include "vklite/util/VkCheckCpp.h"
#include "vklite/util/VkCheck.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/platform/android/instance/VulkanAndroidApi.h"

namespace vklite {

//    HardwareBufferImage::HardwareBufferImage(const PhysicalDevice &physicalDevice,
//                                             const Device &device,
//                                             const HardwareBuffer &androidHardwareBuffer,
//                                             const HardwareBufferYcbcrConversion &conversion)
//            : mDevice(device) {
//        const vk::Device &vkDevice = mDevice.getDevice();
//
//        AHardwareBuffer_Desc hardwareBufferDescription = androidHardwareBuffer.getAndroidHardwareBufferDescription();
//        vk::AndroidHardwareBufferPropertiesANDROID properties = androidHardwareBuffer.getProperties();
//        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo = androidHardwareBuffer.getFormatProperties();
//
//        vk::ExternalMemoryImageCreateInfo externalMemoryImageCreateInfo{};
//        externalMemoryImageCreateInfo
//                .setHandleTypes(vk::ExternalMemoryHandleTypeFlagBitsKHR::eAndroidHardwareBufferANDROID);
//
//        vk::ExternalFormatANDROID externalFormat{};
//        externalFormat
//                .setPNext(&externalMemoryImageCreateInfo);
//        if (formatInfo.format == vk::Format::eUndefined) {
//            externalFormat.externalFormat = formatInfo.externalFormat;
//        }
//
//        // 创建 Vulkan 图像
//        vk::ImageCreateInfo imageCreateInfo{};
//        imageCreateInfo
//                .setPNext(&externalFormat)
//                .setFormat(formatInfo.format)
//                .setFlags(vk::ImageCreateFlags{0})
//                .setImageType(vk::ImageType::e2D)
//                .setExtent(vk::Extent3D(hardwareBufferDescription.width, hardwareBufferDescription.height, 1))
//                .setMipLevels(1)
//                .setArrayLayers(hardwareBufferDescription.layers)
//                .setSamples(vk::SampleCountFlagBits::e1)
//                .setTiling(vk::ImageTiling::eOptimal)
//                .setUsage(vk::ImageUsageFlagBits::eSampled)
//                .setSharingMode(vk::SharingMode::eExclusive)
//                .setQueueFamilyIndices({})
//                .setInitialLayout(vk::ImageLayout::eUndefined);
//
//        mImage = vkDevice.createImage(imageCreateInfo);
//
//        /**
//         * Dedicated: "专用的" / "专属于"
//         * 某块内存专门服务于某个特定的 Vulkan 资源（如 Image 或 Buffer）
//         * 当前分配的内存将专门用于某个特定的 Image 或 Buffer。
//         * 驱动可能会根据此信息优化内存布局或访问路径。
//         */
//        vk::ImportAndroidHardwareBufferInfoANDROID hardwareBufferInfo;
//        hardwareBufferInfo
//                .setBuffer(androidHardwareBuffer.getHardwareBuffer());
//
//        vk::MemoryDedicatedAllocateInfo memoryDedicatedAllocateInfo{};
//        memoryDedicatedAllocateInfo
//                .setImage(mImage)
//                .setBuffer(nullptr)
//                .setPNext(&hardwareBufferInfo);
//
//        // 获取图像的内存需求
//        uint32_t memoryType = physicalDevice.findMemoryType(properties.memoryTypeBits);
//
//        // 分配内存并绑定到图像
//        vk::MemoryAllocateInfo memoryAllocateInfo{};
//        memoryAllocateInfo
//                .setAllocationSize(properties.allocationSize)
//                .setMemoryTypeIndex(memoryType)
//                .setPNext(&memoryDedicatedAllocateInfo);
//
//        mDeviceMemory = vkDevice.allocateMemory(memoryAllocateInfo);
//
//        vk::BindImageMemoryInfo bindImageMemoryInfo;
//        bindImageMemoryInfo
//                .setImage(mImage)
//                .setMemory(mDeviceMemory)
//                .setMemoryOffset(0);
//
//        //device.bindImageMemory2KHR(bindImageMemoryInfo); // link error
//        vkBindImageMemory2KHR(vkDevice, 1, reinterpret_cast<VkBindImageMemoryInfo *>(&bindImageMemoryInfo));
//
//        vk::ImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo{};
//        imageMemoryRequirementsInfo
//                .setImage(mImage);
//
//        vk::MemoryDedicatedRequirements memoryDedicatedRequirements;
//        vk::MemoryRequirements2 memoryRequirements;
//        memoryRequirements.pNext = &memoryDedicatedRequirements;
//
////        device.getImageMemoryRequirements2KHR(&imageMemoryRequirementsInfo, &memoryRequirements); //link error
//        vkGetImageMemoryRequirements2KHR(vkDevice,
//                                         reinterpret_cast<const VkImageMemoryRequirementsInfo2 *>(&imageMemoryRequirementsInfo),
//                                         reinterpret_cast<VkMemoryRequirements2 *>(&memoryRequirements));
//
//        if (!memoryDedicatedRequirements.prefersDedicatedAllocation || !memoryDedicatedRequirements.requiresDedicatedAllocation) {
//            return;
//        }
//
//
//        vk::SamplerYcbcrConversionInfo samplerYcbcrConversionInfo;
//        samplerYcbcrConversionInfo
//                .setConversion(conversion.getSamplerYcbcrConversion());
//
//        vk::ImageSubresourceRange imageSubresourceRange{};
//        imageSubresourceRange
//                .setAspectMask(vk::ImageAspectFlagBits::eColor)
//                .setBaseMipLevel(0)
//                .setLevelCount(1)
//                .setBaseArrayLayer(0)
//                .setLayerCount(1);
//
//        vk::ComponentMapping componentMapping{};
//        componentMapping
//                .setA(vk::ComponentSwizzle::eIdentity)
//                .setR(vk::ComponentSwizzle::eIdentity)
//                .setG(vk::ComponentSwizzle::eIdentity)
//                .setB(vk::ComponentSwizzle::eIdentity);
//
//        vk::ImageViewCreateInfo imageViewCreateInfo;
//        imageViewCreateInfo
//                .setPNext(&samplerYcbcrConversionInfo)
//                .setFormat(formatInfo.format)
//                .setImage(mImage)
//                .setViewType(vk::ImageViewType::e2D)
//                .setComponents(componentMapping)
//                .setSubresourceRange(imageSubresourceRange);
//
//        mImageView = vkDevice.createImageView(imageViewCreateInfo);
//    }
//
//    HardwareBufferImage::~HardwareBufferImage() {
//        const vk::Device &vkDevice = mDevice.getDevice();
//
//        vkDevice.destroyImageView(mImageView);
//        vkDevice.destroyImage(mImage);
//        vkDevice.freeMemory(mDeviceMemory);
//    }
//
//    const vk::Image &HardwareBufferImage::getImage() const {
//        return mImage;
//    }
//
//    [[nodiscard]]
//    const vk::DeviceMemory &HardwareBufferImage::getDeviceMemory() const {
//        return mDeviceMemory;
//    }

} // vklite
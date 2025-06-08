//
// Created by leixing on 2025/6/8.
//

#include "CombinedHardwareBufferImageViewBuilder.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/platform/android/instance/VulkanAndroidApi.h"

namespace vklite {

    CombinedHardwareBufferImageViewBuilder::CombinedHardwareBufferImageViewBuilder()
            : mMemoryOffset(0) {}

    CombinedHardwareBufferImageViewBuilder::~CombinedHardwareBufferImageViewBuilder() = default;

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::device(vk::Device device) {
        mDevice = device;
        mHardwareBufferImageBuilder.device(device);
        mHardwareBufferDeviceMemoryBuilder.device(device);
        mHardwareBufferImageViewBuilder.device(device);
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::hardwareBufferDescription(AHardwareBuffer_Desc hardwareBufferDescription) {
        mHardwareBufferImageBuilder.hardwareBufferDescription(hardwareBufferDescription);
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::hardwareBufferProperties(vk::AndroidHardwareBufferPropertiesANDROID hardwareBufferProperties) {
        mHardwareBufferProperties = hardwareBufferProperties;
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::hardwareBuffer(const HardwareBuffer &hardwareBuffer) {
        hardwareBufferDescription(hardwareBuffer.getAndroidHardwareBufferDescription());
        hardwareBufferProperties(hardwareBuffer.getProperties());
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::hardwareBuffer(AHardwareBuffer *hardwareBuffer) {
        mHardwareBufferDeviceMemoryBuilder.hardwareBuffer(hardwareBuffer);
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::memoryProperties(vk::PhysicalDeviceMemoryProperties memoryProperties) {
        mPhysicalDeviceMemoryProperties = memoryProperties;
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::conversion(const vk::SamplerYcbcrConversion &conversion) {
        mHardwareBufferImageViewBuilder.conversion(conversion);
        return *this;
    }


    CombinedHardwareBufferImageView CombinedHardwareBufferImageViewBuilder::build() {
        if (!mHardwareBufferProperties.has_value()) {
            throw std::runtime_error("CombinedHardwareBufferImageViewBuilder::build() mHardwareBufferProperties not set");
        }
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("CombinedHardwareBufferImageViewBuilder::build() mPhysicalDeviceMemoryProperties not set");
        }

        vk::AndroidHardwareBufferPropertiesANDROID &hardwareBufferProperties = mHardwareBufferProperties.value();
        vk::PhysicalDeviceMemoryProperties &physicalDeviceMemoryProperties = mPhysicalDeviceMemoryProperties.value();
        uint32_t memoryTypeIndex = VulkanUtil::findMemoryTypeIndex(physicalDeviceMemoryProperties, hardwareBufferProperties.memoryTypeBits);

        mHardwareBufferDeviceMemoryBuilder.allocationSize(hardwareBufferProperties.allocationSize);
        mHardwareBufferDeviceMemoryBuilder.memoryTypeIndex(memoryTypeIndex);


        Image image = mHardwareBufferImageBuilder.build();

        DeviceMemory deviceMemory = mHardwareBufferDeviceMemoryBuilder.build();

        ImageView imageView = mHardwareBufferImageViewBuilder
                .image(image.getImage())
                .build();

        vk::BindImageMemoryInfo bindImageMemoryInfo;
        bindImageMemoryInfo
                .setImage(image.getImage())
                .setMemory(deviceMemory.getDeviceMemory())
                .setMemoryOffset(mMemoryOffset);

        //device.bindImageMemory2KHR(bindImageMemoryInfo); // link error
        vkBindImageMemory2KHR(mDevice, 1, reinterpret_cast<VkBindImageMemoryInfo *>(&bindImageMemoryInfo));

        vk::ImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo{};
        imageMemoryRequirementsInfo
                .setImage(image.getImage());

        vk::MemoryDedicatedRequirements memoryDedicatedRequirements;
        vk::MemoryRequirements2 memoryRequirements;
        memoryRequirements.pNext = &memoryDedicatedRequirements;

//        device.getImageMemoryRequirements2KHR(&imageMemoryRequirementsInfo, &memoryRequirements); //link error
        vkGetImageMemoryRequirements2KHR(mDevice,
                                         reinterpret_cast<const VkImageMemoryRequirementsInfo2 *>(&imageMemoryRequirementsInfo),
                                         reinterpret_cast<VkMemoryRequirements2 *>(&memoryRequirements));

        if (!memoryDedicatedRequirements.prefersDedicatedAllocation || !memoryDedicatedRequirements.requiresDedicatedAllocation) {
            throw std::runtime_error("CombinedHardwareBufferImageViewBuilder::build(): memoryDedicatedRequirements error");
        }

        return CombinedHardwareBufferImageView{std::move(image), std::move(deviceMemory), std::move(imageView)};
    }

    std::unique_ptr<CombinedHardwareBufferImageView> CombinedHardwareBufferImageViewBuilder::buildUnique() {
        return std::make_unique<CombinedHardwareBufferImageView>(build());
    }

} // vklite
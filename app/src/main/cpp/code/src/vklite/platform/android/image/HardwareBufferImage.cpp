//
// Created by leixing on 2025/6/9.
//

#include "HardwareBufferImage.h"
#include "vklite/platform/android/api/VulkanAndroidApi.h"

namespace vklite {

    HardwareBufferImage::HardwareBufferImage(const vk::Device &device, const vk::Image &image, const HardwareBufferImageMeta &meta)
            : mDevice(device), mImage(image), mMeta(meta) {}

    HardwareBufferImage::~HardwareBufferImage() {
        if (mDevice != nullptr && mImage != nullptr) {
            mDevice.destroy(mImage);
            mDevice = nullptr;
            mImage = nullptr;
        }
    }

    HardwareBufferImage::HardwareBufferImage(HardwareBufferImage &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mImage(std::exchange(other.mImage, nullptr)),
              mMeta(other.mMeta) {}

    HardwareBufferImage &HardwareBufferImage::operator=(HardwareBufferImage &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mImage = std::exchange(other.mImage, nullptr);
            mMeta = other.mMeta;
        }
        return *this;
    }

    const vk::Image &HardwareBufferImage::getVkImage() const {
        return mImage;
    }

    uint32_t HardwareBufferImage::getMipLevels() const {
        return mMeta.mipLevels;
    }

    vk::Format HardwareBufferImage::getVkFormat() const {
        return mMeta.format;
    }

    uint32_t HardwareBufferImage::getWidth() const {
        return mMeta.extent.width;
    }

    uint32_t HardwareBufferImage::getHeight() const {
        return mMeta.extent.height;
    }

    HardwareBufferImage &HardwareBufferImage::bindMemory(vk::DeviceMemory deviceMemory, vk::DeviceSize memoryOffset) {
        vk::BindImageMemoryInfo bindImageMemoryInfo;
        bindImageMemoryInfo
                .setImage(mImage)
                .setMemory(deviceMemory)
                .setMemoryOffset(memoryOffset);

        //device.bindImageMemory2KHR(bindImageMemoryInfo); // link error
        vkBindImageMemory2KHR(mDevice, 1, reinterpret_cast<VkBindImageMemoryInfo *>(&bindImageMemoryInfo));

        vk::ImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo{};
        imageMemoryRequirementsInfo
                .setImage(mImage);

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

        return *this;
    }

} // vklite
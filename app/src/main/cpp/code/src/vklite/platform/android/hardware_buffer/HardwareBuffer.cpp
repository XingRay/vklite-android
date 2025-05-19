//
// Created by leixing on 2025/3/16.
//

#include "HardwareBuffer.h"
#include "vklite/util/VkCheck.h"

namespace vklite {
    HardwareBuffer::HardwareBuffer(const Device &device, AHardwareBuffer *hardwareBuffer)
            : mDevice(device), mHardwareBuffer(hardwareBuffer) {

    }

    HardwareBuffer::~HardwareBuffer() = default;

    vk::AndroidHardwareBufferPropertiesANDROID HardwareBuffer::getAndroidHardwareBufferProperties() const {
        const vk::Device& vkDevice = mDevice.getDevice();
        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
        propertiesInfo.pNext = &formatInfo;

        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(vkDevice, mHardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&propertiesInfo)));
        return propertiesInfo;
    }

    vk::AndroidHardwareBufferFormatPropertiesANDROID HardwareBuffer::getAndroidHardwareBufferFormatProperties() const {
        const vk::Device& vkDevice = mDevice.getDevice();
        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
        propertiesInfo.pNext = &formatInfo;

        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(vkDevice, mHardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&propertiesInfo)));
        return formatInfo;
    }

    AHardwareBuffer_Desc HardwareBuffer::getAndroidHardwareBufferDescription() const {
        AHardwareBuffer_Desc hardwareBufferDescription;
        AHardwareBuffer_describe(mHardwareBuffer, &hardwareBufferDescription);
        return hardwareBufferDescription;
    }

    uint32_t HardwareBuffer::getDataSize() const {
        AHardwareBuffer_Desc hardwareBufferDescription = getAndroidHardwareBufferDescription();
        return hardwareBufferDescription.width * hardwareBufferDescription.height * hardwareBufferDescription.layers;
    }

    AHardwareBuffer *HardwareBuffer::getHardwareBuffer() const {
        return mHardwareBuffer;
    }

} // vklite
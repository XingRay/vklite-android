//
// Created by leixing on 2025/3/16.
//

#include "HardwareBuffer.h"
#include "vklite/util/VkCheck.h"
#include <utility>

namespace vklite {

    HardwareBuffer::HardwareBuffer(AHardwareBuffer *hardwareBuffer,
                                   vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo,
                                   vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo,
                                   AHardwareBuffer_Desc hardwareBufferDescription)
            : mHardwareBuffer(hardwareBuffer),
              mPropertiesInfo(propertiesInfo),
              mFormatInfo(formatInfo),
              mHardwareBufferDescription(hardwareBufferDescription) {
        mPropertiesInfo.pNext = &mFormatInfo;
    }

    HardwareBuffer::~HardwareBuffer() = default;

    HardwareBuffer::HardwareBuffer(const HardwareBuffer &other) = default;

    HardwareBuffer &HardwareBuffer::operator=(const HardwareBuffer &other) {
        if (this != &other) {
            mHardwareBuffer = other.mHardwareBuffer;
            mPropertiesInfo = other.mPropertiesInfo;
            mFormatInfo = other.mFormatInfo;
            mHardwareBufferDescription = other.mHardwareBufferDescription;

            mPropertiesInfo.pNext = &mFormatInfo;
        }
        return *this;
    }

    HardwareBuffer::HardwareBuffer(HardwareBuffer &&other) noexcept
            : mHardwareBuffer(std::exchange(other.mHardwareBuffer, nullptr)),
              mPropertiesInfo(other.mPropertiesInfo),
              mFormatInfo(other.mFormatInfo),
              mHardwareBufferDescription(other.mHardwareBufferDescription) {
        mPropertiesInfo.pNext = &mFormatInfo;
    }

    HardwareBuffer &HardwareBuffer::operator=(HardwareBuffer &&other) noexcept {
        if (this != &other) {
            mHardwareBuffer = std::exchange(other.mHardwareBuffer, nullptr);
            mPropertiesInfo = other.mPropertiesInfo;
            mFormatInfo = other.mFormatInfo;
            mHardwareBufferDescription = other.mHardwareBufferDescription;

            mPropertiesInfo.pNext = &mFormatInfo;
        }
        return *this;
    }

    vk::AndroidHardwareBufferPropertiesANDROID HardwareBuffer::getProperties() const {
        return mPropertiesInfo;
    }

    vk::AndroidHardwareBufferFormatPropertiesANDROID HardwareBuffer::getFormatProperties() const {
        return mFormatInfo;
    }

    AHardwareBuffer_Desc HardwareBuffer::getAndroidHardwareBufferDescription() const {
        return mHardwareBufferDescription;
    }

    uint32_t HardwareBuffer::getDataSize() const {
        return mHardwareBufferDescription.width * mHardwareBufferDescription.height * mHardwareBufferDescription.layers;
    }

    AHardwareBuffer *HardwareBuffer::getHardwareBuffer() const {
        return mHardwareBuffer;
    }

    HardwareBuffer HardwareBuffer::build(const Device &device, AHardwareBuffer *hardwareBuffer) {
        const vk::Device &vkDevice = device.getDevice();
        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
        propertiesInfo.pNext = &formatInfo;

        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(vkDevice, hardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&propertiesInfo)));

        AHardwareBuffer_Desc hardwareBufferDescription;
        AHardwareBuffer_describe(hardwareBuffer, &hardwareBufferDescription);

//        return HardwareBuffer(hardwareBuffer, propertiesInfo, formatInfo, hardwareBufferDescription);
        return {hardwareBuffer, propertiesInfo, formatInfo, hardwareBufferDescription};
    }
} // vklite
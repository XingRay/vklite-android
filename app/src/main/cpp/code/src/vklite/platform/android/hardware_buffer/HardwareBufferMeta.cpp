//
// Created by leixing on 2025-06-20.
//

#include "HardwareBufferMeta.h"

namespace vklite {

    HardwareBufferMeta::HardwareBufferMeta(vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo,
                                           vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo,
                                           AHardwareBuffer_Desc hardwareBufferDescription)
            : mPropertiesInfo(propertiesInfo),
              mFormatInfo(formatInfo),
              mHardwareBufferDescription(hardwareBufferDescription) {
        mPropertiesInfo.pNext = &mFormatInfo;
    }

    HardwareBufferMeta::~HardwareBufferMeta() = default;

    HardwareBufferMeta::HardwareBufferMeta(const HardwareBufferMeta &other)
            : mPropertiesInfo(other.mPropertiesInfo),
              mFormatInfo(other.mFormatInfo),
              mHardwareBufferDescription(other.mHardwareBufferDescription) {
        mPropertiesInfo.pNext = &mFormatInfo;
    }

    HardwareBufferMeta &HardwareBufferMeta::operator=(const HardwareBufferMeta &other) {
        if (this != &other) {
            mPropertiesInfo = other.mPropertiesInfo;
            mFormatInfo = other.mFormatInfo;
            mHardwareBufferDescription = other.mHardwareBufferDescription;

            mPropertiesInfo.pNext = &mFormatInfo;
        }
        return *this;
    }

    HardwareBufferMeta::HardwareBufferMeta(HardwareBufferMeta &&other) noexcept
            : mPropertiesInfo(other.mPropertiesInfo),
              mFormatInfo(other.mFormatInfo),
              mHardwareBufferDescription(other.mHardwareBufferDescription) {
        mPropertiesInfo.pNext = &mFormatInfo;
    }

    HardwareBufferMeta &HardwareBufferMeta::operator=(HardwareBufferMeta &&other) noexcept {
        if (this != &other) {
            mPropertiesInfo = other.mPropertiesInfo;
            mFormatInfo = other.mFormatInfo;
            mHardwareBufferDescription = other.mHardwareBufferDescription;

            mPropertiesInfo.pNext = &mFormatInfo;
        }
        return *this;
    }

    const vk::AndroidHardwareBufferPropertiesANDROID &HardwareBufferMeta::getPropertiesInfo() const {
        return mPropertiesInfo;
    }

    const vk::AndroidHardwareBufferFormatPropertiesANDROID &HardwareBufferMeta::getFormatInfo() const {
        return mFormatInfo;
    }

    const AHardwareBuffer_Desc &HardwareBufferMeta::getHardwareBufferDescription() const {
        return mHardwareBufferDescription;
    }

    uint32_t HardwareBufferMeta::getDataSize() const {
        return mHardwareBufferDescription.width * mHardwareBufferDescription.height * mHardwareBufferDescription.layers;
    }

} // vklite
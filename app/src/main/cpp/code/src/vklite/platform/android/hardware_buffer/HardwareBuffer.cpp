//
// Created by leixing on 2025/3/16.
//

#include "HardwareBuffer.h"
#include "vklite/util/VkCheck.h"
#include <utility>

namespace vklite {

    HardwareBuffer::HardwareBuffer(AHardwareBuffer *hardwareBuffer,
                                   HardwareBufferMeta meta)
            : mHardwareBuffer(hardwareBuffer),
              mMeta(meta) {}

    HardwareBuffer::~HardwareBuffer() = default;

    HardwareBuffer::HardwareBuffer(const HardwareBuffer &other) = default;

    HardwareBuffer &HardwareBuffer::operator=(const HardwareBuffer &other) {
        if (this != &other) {
            mHardwareBuffer = other.mHardwareBuffer;
            mMeta = other.mMeta;
        }
        return *this;
    }

    HardwareBuffer::HardwareBuffer(HardwareBuffer &&other) noexcept
            : mHardwareBuffer(std::exchange(other.mHardwareBuffer, nullptr)),
              mMeta(std::move(other.mMeta)) {}

    HardwareBuffer &HardwareBuffer::operator=(HardwareBuffer &&other) noexcept {
        if (this != &other) {
            mHardwareBuffer = std::exchange(other.mHardwareBuffer, nullptr);
            mMeta = std::move(other.mMeta);
        }
        return *this;
    }

    vk::AndroidHardwareBufferPropertiesANDROID HardwareBuffer::getProperties() const {
        return mMeta.getPropertiesInfo();
    }

    vk::AndroidHardwareBufferFormatPropertiesANDROID HardwareBuffer::getFormatProperties() const {
        return mMeta.getFormatInfo();
    }

    AHardwareBuffer_Desc HardwareBuffer::getAndroidHardwareBufferDescription() const {
        return mMeta.getHardwareBufferDescription();
    }

    uint32_t HardwareBuffer::getDataSize() const{
        return mMeta.getDataSize();
    }

    AHardwareBuffer *HardwareBuffer::getHardwareBuffer() const {
        return mHardwareBuffer;
    }
    
} // vklite
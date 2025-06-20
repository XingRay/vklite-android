//
// Created by leixing on 2025/3/16.
//

#include "HardwareBufferBuilder.h"
#include "vklite/util/VkCheck.h"
#include <utility>

namespace vklite {

    HardwareBufferBuilder::HardwareBufferBuilder()
            : mHardwareBuffer(nullptr) {}

    HardwareBufferBuilder::~HardwareBufferBuilder() = default;

    HardwareBufferBuilder &HardwareBufferBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    HardwareBufferBuilder &HardwareBufferBuilder::hardwareBuffer(AHardwareBuffer *hardwareBuffer) {
        mHardwareBuffer = hardwareBuffer;
        return *this;
    }

    HardwareBuffer HardwareBufferBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("HardwareBufferBuilder::build(): mDevice == nullptr");
        }
        if (mHardwareBuffer == nullptr) {
            throw std::runtime_error("HardwareBufferBuilder::build(): mHardwareBuffer == nullptr");
        }

        // 获取 HardwareBufferBuilder 属性
        vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
        propertiesInfo.pNext = &formatInfo;

        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(mDevice, mHardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&propertiesInfo)));

        AHardwareBuffer_Desc hardwareBufferDescription;
        AHardwareBuffer_describe(mHardwareBuffer, &hardwareBufferDescription);

        HardwareBufferMeta meta(propertiesInfo, formatInfo, hardwareBufferDescription);
        return HardwareBuffer{mHardwareBuffer, meta};
    }

    std::unique_ptr<HardwareBuffer> HardwareBufferBuilder::buildUnique() {
        return std::make_unique<HardwareBuffer>(build());
    }

} // vklite
//
// Created by leixing on 2025/5/30.
//

#include "CombinedMemoryImage.h"

namespace vklite {

    CombinedMemoryImage::CombinedMemoryImage(Image &&image, DeviceMemory &&deviceMemory)
            : mImage(std::move(image)),
              mDeviceMemory(std::move(deviceMemory)) {}

    CombinedMemoryImage::~CombinedMemoryImage() = default;

    CombinedMemoryImage::CombinedMemoryImage(CombinedMemoryImage &&other) noexcept
            : mImage(std::move(other.mImage)),
              mDeviceMemory(std::move(other.mDeviceMemory)) {}

    CombinedMemoryImage &CombinedMemoryImage::operator=(CombinedMemoryImage &&other) noexcept {
        if (this != &other) {
            mImage = std::move(other.mImage);
            mDeviceMemory = std::move(other.mDeviceMemory);
        }
        return *this;
    }

    [[nodiscard]]
    Image &CombinedMemoryImage::getImage() {
        return mImage;
    }

    [[nodiscard]]
    DeviceMemory &CombinedMemoryImage::getDeviceMemory() {
        return mDeviceMemory;
    }

} // vklite
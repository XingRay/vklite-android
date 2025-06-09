//
// Created by leixing on 2025/6/8.
//

#include "CombinedHardwareBufferImageView.h"

namespace vklite {

    CombinedHardwareBufferImageView::CombinedHardwareBufferImageView(HardwareBufferImage &&image, DeviceMemory &&deviceMemory, ImageView &&imageView)
            : mImage(std::move(image)),
              mDeviceMemory(std::move(deviceMemory)),
              mImageView(std::move(imageView)) {}

    CombinedHardwareBufferImageView::~CombinedHardwareBufferImageView() = default;

    CombinedHardwareBufferImageView::CombinedHardwareBufferImageView(CombinedHardwareBufferImageView &&other) noexcept
            : mImage(std::move(other.mImage)),
              mDeviceMemory(std::move(other.mDeviceMemory)),
              mImageView(std::move(other.mImageView)) {}

    CombinedHardwareBufferImageView &CombinedHardwareBufferImageView::operator=(CombinedHardwareBufferImageView &&other) noexcept {
        if (this != &other) {
            mImage = std::move(other.mImage);
            mDeviceMemory = std::move(other.mDeviceMemory);
            mImageView = std::move(other.mImageView);
        }
        return *this;
    }

    const ImageView &CombinedHardwareBufferImageView::getImageView() const {
        return mImageView;
    }

} // vklite
//
// Created by leixing on 2025/5/30.
//

#include "CombinedImageView.h"

namespace vklite {

    CombinedImageView::CombinedImageView(Image &&image, DeviceMemory &&deviceMemory, ImageView &&imageView)
            : mImage(std::move(image)), mDeviceMemory(std::move(deviceMemory)), mImageView(std::move(imageView)) {}

    CombinedImageView::~CombinedImageView() = default;

    CombinedImageView::CombinedImageView(CombinedImageView &&other) noexcept
            : mImage(std::move(other.mImage)),
              mDeviceMemory(std::move(other.mDeviceMemory)),
              mImageView(std::move(other.mImageView)) {}

    CombinedImageView &CombinedImageView::operator=(CombinedImageView &&other) noexcept {
        if (this != &other) {
            mImage = std::move(other.mImage);
            mDeviceMemory = std::move(other.mDeviceMemory);
            mImageView = std::move(other.mImageView);
        }
        return *this;
    }

    [[nodiscard]]
    Image &CombinedImageView::getImage() {
        return mImage;
    }

    [[nodiscard]]
    DeviceMemory &CombinedImageView::getDeviceMemory() {
        return mDeviceMemory;
    }

    [[nodiscard]]
    ImageView &CombinedImageView::getImageView() {
        return mImageView;
    }

} // vklite
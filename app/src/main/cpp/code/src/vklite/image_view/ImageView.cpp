//
// Created by leixing on 2025/5/7.
//

#include "ImageView.h"
#include "vklite/Log.h"

#include <utility>

namespace vklite {

    ImageView::ImageView(vk::Device device, vk::ImageView imageView)
            : mDevice(device), mImageView(imageView) {}

    ImageView::~ImageView() {
        if (mDevice != nullptr && mImageView != nullptr) {
            mDevice.destroy(mImageView);
            mDevice = nullptr;
            mImageView = nullptr;
        }
    }

    ImageView::ImageView(ImageView &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mImageView(std::exchange(other.mImageView, nullptr)) {}

    ImageView &ImageView::operator=(ImageView &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mImageView = std::exchange(other.mImageView, nullptr);
        }
        return *this;
    }

    const vk::ImageView &ImageView::getVkImageView() const {
        return mImageView;
    }

} // vklite
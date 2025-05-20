//
// Created by leixing on 2025/5/7.
//

#include "ImageView.h"
#include "vklite/Log.h"

namespace vklite {

    ImageView::ImageView(const Device &device, const vk::ImageViewCreateInfo &imageViewCreateInfo)
            : mDevice(device) {

        mImageView = mDevice.getDevice().createImageView(imageViewCreateInfo);
    }

    ImageView::ImageView(ImageView &&other) noexcept
            : mDevice(other.mDevice),
              mImageView(std::exchange(other.mImageView, nullptr)) {}

    ImageView::~ImageView() {
        if (mImageView != nullptr) {
            const vk::Device &vkDevice = mDevice.getDevice();
            vkDevice.destroy(mImageView);
        }
    }

    const vk::ImageView &ImageView::getImageView() const {
        return mImageView;
    }

} // vklite
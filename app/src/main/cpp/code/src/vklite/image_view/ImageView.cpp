//
// Created by leixing on 2025/5/7.
//

#include "ImageView.h"
#include "vklite/Log.h"

namespace vklite {

    ImageView::ImageView(const Device &device, const vk::Image &image, vk::Format format, vk::ImageAspectFlags imageAspectFlags, uint32_t levelCount)
            : mDevice(device) {

        vk::ImageSubresourceRange imageSubresourceRange;
        imageSubresourceRange
                .setAspectMask(imageAspectFlags)
                .setBaseMipLevel(0)
                .setLevelCount(levelCount)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        vk::ComponentMapping componentMapping;
        componentMapping
                .setR(vk::ComponentSwizzle::eIdentity)
                .setG(vk::ComponentSwizzle::eIdentity)
                .setB(vk::ComponentSwizzle::eIdentity)
                .setA(vk::ComponentSwizzle::eIdentity);

        vk::ImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo
                .setImage(image)
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(format)
                .setSubresourceRange(imageSubresourceRange)
                .setComponents(componentMapping);

        mImageView = mDevice.getDevice().createImageView(imageViewCreateInfo);
    }

    ImageView::ImageView(ImageView &&other) noexcept
            : mDevice(other.mDevice),
              mImageView(std::exchange(other.mImageView, nullptr)) {}

    ImageView::~ImageView() {
        LOG_D("ImageView::~ImageView");
        if (mImageView != nullptr) {
            LOG_D("vkDevice.destroy(mImageView); mImageView:%p", static_cast<void *>(mImageView));
            const vk::Device &vkDevice = mDevice.getDevice();
            vkDevice.destroy(mImageView);
        } else {
            LOG_D("mImageView is null, 'vkDevice.destroy(mImageView);' skipped");
        }
    }

    const vk::ImageView &ImageView::getImageView() const {
        return mImageView;
    }

} // vklite
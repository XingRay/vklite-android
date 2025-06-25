//
// Created by leixing on 2025/5/7.
//

#include "ImageViewBuilder.h"
#include "vklite/Log.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    ImageViewBuilder::ImageViewBuilder()
            : mImageViewCreateInfo{} {}

    ImageViewBuilder::~ImageViewBuilder() = default;

    ImageViewBuilder &ImageViewBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::image(vk::Image image) {
        mImageViewCreateInfo.image = image;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::aspectMask(vk::ImageAspectFlags aspectMask) {
        mImageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::baseMipLevel(uint32_t baseMipLevel) {
        mImageViewCreateInfo.subresourceRange.baseMipLevel = baseMipLevel;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::levelCount(uint32_t levelCount) {
        mImageViewCreateInfo.subresourceRange.levelCount = levelCount;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::baseArrayLayer(uint32_t baseArrayLayer) {
        mImageViewCreateInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::layerCount(uint32_t layerCount) {
        mImageViewCreateInfo.subresourceRange.layerCount = layerCount;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::componentMappingRed(vk::ComponentSwizzle mapping) {
        mImageViewCreateInfo.components.r = mapping;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::componentMappingGreen(vk::ComponentSwizzle mapping) {
        mImageViewCreateInfo.components.g = mapping;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::componentMappingBlue(vk::ComponentSwizzle mapping) {
        mImageViewCreateInfo.components.b = mapping;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::componentMappingAlpha(vk::ComponentSwizzle mapping) {
        mImageViewCreateInfo.components.a = mapping;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::viewType(vk::ImageViewType viewType) {
        mImageViewCreateInfo.viewType = viewType;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::format(vk::Format format) {
        mImageViewCreateInfo.format = format;
        return *this;
    }

    ImageViewBuilder &ImageViewBuilder::next(const void *next) {
        mImageViewCreateInfo.pNext = next;
        return *this;
    }

    ImageView ImageViewBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("ImageViewBuilder::build(): mDevice == nullptr");
        }
        if (mImageViewCreateInfo.image == nullptr) {
            throw std::runtime_error("ImageViewBuilder::build(): mImageViewCreateInfo.image == nullptr");
        }

//        LOG_D("mDevice.createImageView(mImageViewCreateInfo); mImageViewCreateInfo:");
//        LOG_D("\timage        : %p", (void *) mImageViewCreateInfo.image);
//        LOG_D("\tviewType     : %s", VulkanUtil::toString(mImageViewCreateInfo.viewType).c_str());
//        LOG_D("\tformat       : %s", VulkanUtil::toString(mImageViewCreateInfo.format).c_str());
//        LOG_D("\tcomponents");
//        LOG_D("\t\tr:%s", VulkanUtil::toString(mImageViewCreateInfo.components.r).c_str());
//        LOG_D("\t\tg:%s", VulkanUtil::toString(mImageViewCreateInfo.components.g).c_str());
//        LOG_D("\t\tb:%s", VulkanUtil::toString(mImageViewCreateInfo.components.b).c_str());
//        LOG_D("\t\ta:%s", VulkanUtil::toString(mImageViewCreateInfo.components.a).c_str());
//        LOG_D("\tsubresourceRange:");
//        LOG_D("\t\taspectMask:%s", VulkanUtil::toString(mImageViewCreateInfo.subresourceRange.aspectMask).c_str());
//        LOG_D("\t\tbaseMipLevel:%d", mImageViewCreateInfo.subresourceRange.baseMipLevel);
//        LOG_D("\t\tlevelCount:%d", mImageViewCreateInfo.subresourceRange.levelCount);
//        LOG_D("\t\tbaseArrayLayer:%d", mImageViewCreateInfo.subresourceRange.baseArrayLayer);
//        LOG_D("\t\tlayerCount:%d", mImageViewCreateInfo.subresourceRange.layerCount);


        vk::ImageView imageView = mDevice.createImageView(mImageViewCreateInfo);
//        LOG_D("mDevice.createImageView(mImageViewCreateInfo) => %p", (void *) imageView);

        return ImageView{mDevice, imageView};
    }

    std::unique_ptr<ImageView> ImageViewBuilder::buildUnique() {
        return std::make_unique<ImageView>(build());
    }

    std::vector<ImageView> ImageViewBuilder::build(const std::vector<vk::Image> &images) {
        std::vector<ImageView> imageViews;
        imageViews.reserve(images.size());

        for (const vk::Image &image: images) {
            this->image(image);
            imageViews.emplace_back(build());
        }

        return imageViews;
    }

    ImageViewBuilder &ImageViewBuilder::asDefault() {
        return (*this)
                .baseMipLevel(0)
                .levelCount(1)
                .baseArrayLayer(0)
                .layerCount(1)
                .componentMappingRed(vk::ComponentSwizzle::eIdentity)
                .componentMappingGreen(vk::ComponentSwizzle::eIdentity)
                .componentMappingBlue(vk::ComponentSwizzle::eIdentity)
                .componentMappingAlpha(vk::ComponentSwizzle::eIdentity)
                .viewType(vk::ImageViewType::e2D);
    }

    ImageViewBuilder &ImageViewBuilder::asColorAttachment() {
        return (*this)
                .asDefault()
                .aspectMask(vk::ImageAspectFlagBits::eColor);
    }

    ImageViewBuilder &ImageViewBuilder::asDepthAttachment() {
        return (*this)
                .asDefault()
                .aspectMask(vk::ImageAspectFlagBits::eDepth);
    }

    ImageViewBuilder &ImageViewBuilder::asTexture() {
        return (*this)
                .asDefault()
                .aspectMask(vk::ImageAspectFlagBits::eColor);
    }

    ImageViewBuilder &ImageViewBuilder::asStorageImage() {
        return (*this)
                .asDefault()
                .aspectMask(vk::ImageAspectFlagBits::eColor);
    }

} // vklite
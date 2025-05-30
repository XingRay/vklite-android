//
// Created by leixing on 2025/4/11.
//

#include "ImageBuilder.h"
#include "Image.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/image/ImageMeta.h"

namespace vklite {

    ImageBuilder::ImageBuilder()
            : mImageCreateInfo{} {}

    ImageBuilder::~ImageBuilder() = default;

    ImageBuilder &ImageBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    ImageBuilder &ImageBuilder::width(uint32_t width) {
        mImageCreateInfo.extent.width = width;
        return *this;
    }

    ImageBuilder &ImageBuilder::height(uint32_t height) {
        mImageCreateInfo.extent.height = height;
        return *this;
    }

    ImageBuilder &ImageBuilder::size(vk::Extent2D size) {
        (*this)
                .width(size.width)
                .height(size.height);
        return *this;
    }

    ImageBuilder &ImageBuilder::depth(uint32_t depth) {
        mImageCreateInfo.extent.depth = depth;
        return *this;
    }

    ImageBuilder &ImageBuilder::extent(vk::Extent3D extent) {
        mImageCreateInfo.extent = extent;
        return *this;
    }

    ImageBuilder &ImageBuilder::imageType(vk::ImageType imageType) {
        mImageCreateInfo.imageType = imageType;
        return *this;
    }

    ImageBuilder &ImageBuilder::mipLevels(uint32_t mipLevels) {
        mImageCreateInfo.mipLevels = mipLevels;
        return *this;
    }

    ImageBuilder &ImageBuilder::arrayLayers(uint32_t arrayLayers) {
        mImageCreateInfo.arrayLayers = arrayLayers;
        return *this;
    }

    ImageBuilder &ImageBuilder::format(vk::Format format) {
        mImageCreateInfo.format = format;
        return *this;
    }

//        //VK_IMAGE_TILING_LINEAR texel按行的顺序排列
//        //VK_IMAGE_TILING_OPTIMAL texel按实现定义的顺序排列
//        //the tiling mode cannot be changed at a later time.
//        //如果希望能够直接访问图像内存中的texel，则必须使用VK_IMAGE_TILING_LINEAR
    ImageBuilder &ImageBuilder::tiling(vk::ImageTiling tiling) {
        mImageCreateInfo.tiling = tiling;
        return *this;
    }

//        //VK_IMAGE_LAYOUT_UNDEFINED 不能被 GPU 使用，并且第一个转换将丢弃texel
//        //VK_IMAGE_TILING_OPTIMAL 不能被 GPU 使用，并且第一个转换将保留texel
    ImageBuilder &ImageBuilder::initialLayout(vk::ImageLayout initialLayout) {
        mImageCreateInfo.initialLayout = initialLayout;
        return *this;
    }

    ImageBuilder &ImageBuilder::usage(vk::ImageUsageFlags usage) {
        mImageCreateInfo.usage = usage;
        return *this;
    }

    ImageBuilder &ImageBuilder::sharingMode(vk::SharingMode sharingMode) {
        mImageCreateInfo.sharingMode = sharingMode;
        return *this;
    }

    ImageBuilder &ImageBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mImageCreateInfo.samples = sampleCount;
        return *this;
    }

    //用于稀疏纹理相关的标志位
    ImageBuilder &ImageBuilder::flags(vk::ImageCreateFlags flags) {
        mImageCreateInfo.flags = flags;
        return *this;
    }

    Image ImageBuilder::build() {
        vk::Image image = mDevice.createImage(mImageCreateInfo);
        ImageMeta meta{mImageCreateInfo.format, mImageCreateInfo.extent, mImageCreateInfo.mipLevels};
//        return Image(mDevice, image, meta);
        return {mDevice, image, meta};
    }

    [[nodiscard]]
    std::unique_ptr<Image> ImageBuilder::buildUnique() {
        return std::make_unique<Image>(build());
    }

    ImageBuilder &ImageBuilder::asDefault() {
        return (*this)
                .depth(1)
                .mipLevels(1)
                .imageType(vk::ImageType::e2D)
                .arrayLayers(1)
                .tiling(vk::ImageTiling::eOptimal)
                .initialLayout(vk::ImageLayout::eUndefined)
                .sharingMode(vk::SharingMode::eExclusive)
                .sampleCount(vk::SampleCountFlagBits::e1)
                .flags(vk::ImageCreateFlags{});
    }

    ImageBuilder &ImageBuilder::asColorImageBuilder() {
        return (*this)
                .asDefault()
                .usage(vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment);
    }

    ImageBuilder &ImageBuilder::asDepthImageBuilder() {
        return (*this)
                .asDefault()
                .usage(vk::ImageUsageFlagBits::eDepthStencilAttachment);
    }

    ImageBuilder &ImageBuilder::asTextureImageBuilder() {
        return (*this)
                .asDefault()
                .usage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
                .sampleCount(vk::SampleCountFlagBits::e1);
    }

    ImageBuilder ImageBuilder::defaultImageBuilder() {
        return ImageBuilder().asDefault();
    }

    ImageBuilder ImageBuilder::colorImageBuilder() {
        return ImageBuilder().asColorImageBuilder();
    }

    ImageBuilder ImageBuilder::depthImageBuilder() {
        return ImageBuilder().asDepthImageBuilder();
    }

    ImageBuilder ImageBuilder::textureImageBuilder() {
        return ImageBuilder().asTextureImageBuilder();
    }

} // vklite
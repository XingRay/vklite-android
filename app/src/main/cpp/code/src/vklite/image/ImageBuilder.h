//
// Created by leixing on 2025/4/11.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/image/Image.h"


namespace vklite {

    class ImageBuilder {
    private:
        vk::Device mDevice;
        vk::ImageCreateInfo mImageCreateInfo;

        std::vector<uint32_t> mQueueFamilyIndices;

    public:
        ImageBuilder();

        ~ImageBuilder();

        ImageBuilder &device(vk::Device device);

        ImageBuilder &width(uint32_t width);

        ImageBuilder &height(uint32_t height);

        ImageBuilder &size(uint32_t width, uint32_t height);

        ImageBuilder &size(vk::Extent2D size);

        ImageBuilder &depth(uint32_t depth);

        ImageBuilder &extent(vk::Extent3D extent);

        ImageBuilder &imageType(vk::ImageType imageType);

        ImageBuilder &mipLevels(uint32_t mipLevels);

        ImageBuilder &arrayLayers(uint32_t arrayLayers);

        ImageBuilder &format(vk::Format format);

//        //VK_IMAGE_TILING_LINEAR texel按行的顺序排列
//        //VK_IMAGE_TILING_OPTIMAL texel按实现定义的顺序排列
//        //the tiling mode cannot be changed at a later time.
//        //如果希望能够直接访问图像内存中的texel，则必须使用VK_IMAGE_TILING_LINEAR
        ImageBuilder &tiling(vk::ImageTiling tiling);

//        //VK_IMAGE_LAYOUT_UNDEFINED 不能被 GPU 使用，并且第一个转换将丢弃texel
//        //VK_IMAGE_TILING_OPTIMAL 不能被 GPU 使用，并且第一个转换将保留texel
        ImageBuilder &initialLayout(vk::ImageLayout initialLayout);

        ImageBuilder &usage(vk::ImageUsageFlags usage);

        ImageBuilder &addUsage(vk::ImageUsageFlagBits usage);

        ImageBuilder &sharingMode(vk::SharingMode sharingMode);

        ImageBuilder &sampleCount(vk::SampleCountFlagBits sampleCount);

        ImageBuilder &queueFamilyIndices(std::vector<uint32_t> &&queueFamilyIndices);

        //用于稀疏纹理相关的标志位
        ImageBuilder &flags(vk::ImageCreateFlags flags);

        ImageBuilder &next(const void *next);

        [[nodiscard]]
        Image build();

        [[nodiscard]]
        std::unique_ptr<Image> buildUnique();

        ImageBuilder &asDefault();

        ImageBuilder &asColorAttachment();

        ImageBuilder &asDepthAttachment();

        ImageBuilder &asTexture();

        ImageBuilder &asStorageImage();
    };

} // vklite

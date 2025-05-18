//
// Created by leixing on 2025/4/11.
//

#pragma once

#include "ImageBuilderInterface.h"
#include "vklite/image/Image.h"

namespace vklite {

    class ImageBuilder {
    private:
        uint32_t mWidth;
        uint32_t mHeight;
        uint32_t mMipLevels;
        vk::Format mFormat;
        vk::ImageUsageFlags mImageUsageFlags;
        vk::SampleCountFlagBits mSampleCount;
        vk::ImageTiling mImageTiling;

    public:
        ImageBuilder();

        ~ImageBuilder();

        ImageBuilder &width(uint32_t width);

        ImageBuilder &height(uint32_t height);

        ImageBuilder &mipLevels(uint32_t mipLevels);

        ImageBuilder &format(vk::Format format);

        ImageBuilder &imageUsageFlags(vk::ImageUsageFlags imageUsageFlags);

        ImageBuilder &sampleCount(vk::SampleCountFlagBits sampleCount);

        ImageBuilder &imageTiling(vk::ImageTiling imageTiling);

        [[nodiscard]]
        std::unique_ptr<Image> buildUnique(const Device &device);

    public:// static
        static ImageBuilder colorImageBuilder();

        static ImageBuilder depthImageBuilder();

        static ImageBuilder textureImageBuilder();
    };

} // vklite

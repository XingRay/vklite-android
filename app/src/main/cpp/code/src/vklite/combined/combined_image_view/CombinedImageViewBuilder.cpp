//
// Created by leixing on 2025/5/30.
//

#include "CombinedImageViewBuilder.h"

namespace vklite {

    CombinedImageViewBuilder::CombinedImageViewBuilder()
            : mMemoryOffset(0) {}

    CombinedImageViewBuilder::~CombinedImageViewBuilder() = default;

    CombinedImageViewBuilder &CombinedImageViewBuilder::device(vk::Device device) {
        mImageBuilder.device(device);
        mDeviceMemoryBuilder.device(device);
        mImageViewBuilder.device(device);
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::memoryOffset(vk::DeviceSize memoryOffset) {
        mMemoryOffset = memoryOffset;
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::imageBuilder(std::function<void(ImageBuilder &builder)> &&configure) {
        mImageBuilderConfigure = std::move(configure);
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::deviceMemoryBuilder(std::function<void(Image &image, DeviceMemoryBuilder &builder)> &&configure) {
        mDeviceMemoryBuilderConfigure = std::move(configure);
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::imageViewBuilder(std::function<void(Image &image, ImageViewBuilder &builder)> &&configure) {
        mImageViewBuilderConfigure = std::move(configure);
        return *this;
    }

    CombinedImageView CombinedImageViewBuilder::build() {
        mImageBuilderConfigure(mImageBuilder);
        Image image = mImageBuilder.build();

        mDeviceMemoryBuilderConfigure(image, mDeviceMemoryBuilder);
        DeviceMemory deviceMemory = mDeviceMemoryBuilder.build();

        image.bindMemory(deviceMemory.getDeviceMemory(), mMemoryOffset);

        mImageViewBuilderConfigure(image, mImageViewBuilder);
        ImageView imageView = mImageViewBuilder.build();

//        return CombinedMemoryImage(std::move(image), std::move(deviceMemory), std::move(imageView));
        return {std::move(image), std::move(deviceMemory), std::move(imageView)};
    }

    std::unique_ptr<CombinedImageView> CombinedImageViewBuilder::buildUnique() {
        return std::make_unique<CombinedImageView>(build());
    }

} // vklite
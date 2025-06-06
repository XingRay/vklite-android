//
// Created by leixing on 2025/5/30.
//

#include "CombinedImageViewBuilder.h"
#include "vklite/Log.h"

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

    CombinedImageViewBuilder &CombinedImageViewBuilder::format(vk::Format format) {
        mImageBuilder.format(format);
        mImageViewBuilder.format(format);
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::size(vk::Extent2D size) {
        mImageBuilder.size(size);
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mImageBuilder.sampleCount(sampleCount);
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::memoryOffset(vk::DeviceSize memoryOffset) {
        mMemoryOffset = memoryOffset;
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                                                                           vk::MemoryPropertyFlags memoryPropertyFlags) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        mMemoryPropertyFlags = memoryPropertyFlags;
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
        LOG_D("CombinedImageViewBuilder::build()");

        // create Image
        if (mImageBuilderConfigure != nullptr) {
            mImageBuilderConfigure(mImageBuilder);
        }
        Image image = mImageBuilder.build();

        // create DeviceMemory
        if (mPhysicalDeviceMemoryProperties.has_value()) {
            mDeviceMemoryBuilder.config(mPhysicalDeviceMemoryProperties.value(), image.getImage(), mMemoryPropertyFlags);
        }
        if (mDeviceMemoryBuilderConfigure != nullptr) {
            mDeviceMemoryBuilderConfigure(image, mDeviceMemoryBuilder);
        }
        DeviceMemory deviceMemory = mDeviceMemoryBuilder.build();

        // bind memory
        image.bindMemory(deviceMemory.getDeviceMemory(), mMemoryOffset);

        // create ImageView
        mImageViewBuilder.image(image.getImage());
        if (mImageViewBuilderConfigure != nullptr) {
            mImageViewBuilderConfigure(image, mImageViewBuilder);
        }
        ImageView imageView = mImageViewBuilder.build();

        // combine Image / DeviceMemory / ImageView as CombinedMemoryImage
//        return CombinedMemoryImage(std::move(image), std::move(deviceMemory), std::move(imageView));
        return {std::move(image), std::move(deviceMemory), std::move(imageView)};
    }

    std::unique_ptr<CombinedImageView> CombinedImageViewBuilder::buildUnique() {
        return std::make_unique<CombinedImageView>(build());
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::asDefault() {
        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::asColor() {
        asDefault();
        mImageBuilder.asColorImageBuilder();
        mImageViewBuilder.asColorImageViewBuilder();

        return *this;
    }

    CombinedImageViewBuilder &CombinedImageViewBuilder::asDepth() {
        asDefault();
        mImageBuilder.asDepthImageBuilder();
        mImageViewBuilder.asDepthImageViewBuilder();

        return *this;
    }

} // vklite
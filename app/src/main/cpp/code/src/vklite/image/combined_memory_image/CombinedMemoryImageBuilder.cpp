//
// Created by leixing on 2025/5/30.
//

#include "CombinedMemoryImageBuilder.h"

namespace vklite {

    CombinedMemoryImageBuilder::CombinedMemoryImageBuilder()
            : mMemoryOffset(0) {}

    CombinedMemoryImageBuilder::~CombinedMemoryImageBuilder() = default;

    CombinedMemoryImageBuilder &CombinedMemoryImageBuilder::device(vk::Device device) {
        mImageBuilder.device(device);
        mDeviceMemoryBuilder.device(device);
        return *this;
    }

    CombinedMemoryImageBuilder &CombinedMemoryImageBuilder::memoryOffset(vk::DeviceSize memoryOffset) {
        mMemoryOffset = memoryOffset;
        return *this;
    }

    CombinedMemoryImageBuilder &CombinedMemoryImageBuilder::imageBuilder(std::function<void(ImageBuilder &builder)> &&configure) {
        mImageBuilderConfigure = std::move(configure);
        return *this;
    }

    CombinedMemoryImageBuilder &CombinedMemoryImageBuilder::deviceMemoryBuilder(std::function<void(Image &image, DeviceMemoryBuilder &builder)> &&configure) {
        mDeviceMemoryBuilderConfigure = std::move(configure);
        return *this;
    }

    CombinedMemoryImage CombinedMemoryImageBuilder::build() {
        mImageBuilderConfigure(mImageBuilder);
        Image image = mImageBuilder.build();

        mDeviceMemoryBuilderConfigure(image, mDeviceMemoryBuilder);
        DeviceMemory deviceMemory = mDeviceMemoryBuilder.build();

        image.bindMemory(deviceMemory.getVkDeviceMemory(), mMemoryOffset);

        return CombinedMemoryImage{std::move(image), std::move(deviceMemory)};
    }

    std::unique_ptr<CombinedMemoryImage> CombinedMemoryImageBuilder::buildUnique() {
        return std::make_unique<CombinedMemoryImage>(build());
    }

} // vklite
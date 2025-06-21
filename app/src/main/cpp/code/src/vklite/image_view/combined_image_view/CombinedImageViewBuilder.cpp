//
// Created by leixing on 2025/5/30.
//

#include "CombinedImageViewBuilder.h"
#include "vklite/Log.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    CombinedImageViewBuilder::CombinedImageViewBuilder()
            : mMemoryOffset(0) {}

    CombinedImageViewBuilder::~CombinedImageViewBuilder() = default;

    CombinedImageViewBuilder &CombinedImageViewBuilder::device(vk::Device device) {
        mDevice = device;

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

    CombinedImageView CombinedImageViewBuilder::build() {
        LOG_D("CombinedImageViewBuilder::build()");

        // create Image
        Image image = mImageBuilder.build();

        // create DeviceMemory
        vk::MemoryRequirements memoryRequirements = mDevice.getImageMemoryRequirements(image.getImage());
        mDeviceMemoryBuilder.allocationSize(memoryRequirements.size);

        uint32_t memoryTypeIndex = VulkanUtil::findMemoryTypeIndex(mPhysicalDeviceMemoryProperties, memoryRequirements, mMemoryPropertyFlags);
        mDeviceMemoryBuilder.memoryTypeIndex(memoryTypeIndex);

        DeviceMemory deviceMemory = mDeviceMemoryBuilder.build();

        // bind memory
        image.bindMemory(deviceMemory.getDeviceMemory(), mMemoryOffset);

        // create ImageView
        mImageViewBuilder.image(image.getImage());
        ImageView imageView = mImageViewBuilder.build();

        // combine Image / DeviceMemory / ImageView as CombinedMemoryImage
        return CombinedImageView{std::move(image), std::move(deviceMemory), std::move(imageView)};
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
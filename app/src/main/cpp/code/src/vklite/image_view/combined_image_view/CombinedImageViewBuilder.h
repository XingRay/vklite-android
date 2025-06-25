//
// Created by leixing on 2025/5/30.
//

#pragma once

#include <functional>
#include <memory>

#include "CombinedImageView.h"
#include "vklite/image/ImageBuilder.h"
#include "vklite/device_memory/DeviceMemoryBuilder.h"
#include "vklite/image_view/ImageViewBuilder.h"

namespace vklite {

    class CombinedImageViewBuilder {
    private:
        vk::Device mDevice;

        ImageBuilder mImageBuilder;

        DeviceMemoryBuilder mDeviceMemoryBuilder;

        vk::PhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
        vk::MemoryPropertyFlags mMemoryPropertyFlags;

        // bindMemory param
        vk::DeviceSize mMemoryOffset;

        ImageViewBuilder mImageViewBuilder;

        std::function<void(ImageBuilder &builder)> mImageBuilderConfigure;
        std::function<void(Image &image, DeviceMemoryBuilder &builder)> mDeviceMemoryBuilderConfigure;
        std::function<void(Image &image, ImageViewBuilder &builder)> mImageViewBuilderConfigure;

    public:

        CombinedImageViewBuilder();

        ~CombinedImageViewBuilder();

        CombinedImageViewBuilder &device(vk::Device device);

        CombinedImageViewBuilder &format(vk::Format format);

        CombinedImageViewBuilder &size(vk::Extent2D size);

        CombinedImageViewBuilder &size(uint32_t width, uint32_t height);

        CombinedImageViewBuilder &sampleCount(vk::SampleCountFlagBits sampleCount);

        CombinedImageViewBuilder &memoryOffset(vk::DeviceSize memoryOffset);

        CombinedImageViewBuilder &physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        CombinedImageViewBuilder &memoryProperty(vk::MemoryPropertyFlags memoryProperty);


        CombinedImageView build();

        std::vector<CombinedImageView> build(uint32_t count);

        std::unique_ptr<CombinedImageView> buildUnique();


        CombinedImageViewBuilder &asDefault();

        CombinedImageViewBuilder &asColorAttachment();

        CombinedImageViewBuilder &asDepthAttachment();

        CombinedImageViewBuilder &asStorageImage();
    };

} // vklite

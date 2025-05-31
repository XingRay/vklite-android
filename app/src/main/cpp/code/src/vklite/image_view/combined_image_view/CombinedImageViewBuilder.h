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
        ImageBuilder mImageBuilder;

        DeviceMemoryBuilder mDeviceMemoryBuilder;
        vk::DeviceSize mMemoryOffset;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;
        vk::MemoryPropertyFlags mMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;

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

        CombinedImageViewBuilder &sampleCount(vk::SampleCountFlagBits sampleCount);

        CombinedImageViewBuilder &memoryOffset(vk::DeviceSize memoryOffset);

        CombinedImageViewBuilder &configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                                                     vk::MemoryPropertyFlags memoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal);

        CombinedImageViewBuilder &imageBuilder(std::function<void(ImageBuilder &builder)> &&configure);

        CombinedImageViewBuilder &deviceMemoryBuilder(std::function<void(Image &image, DeviceMemoryBuilder &builder)> &&configure);

        CombinedImageViewBuilder &imageViewBuilder(std::function<void(Image &image, ImageViewBuilder &builder)> &&configure);

        CombinedImageView build();

        std::unique_ptr<CombinedImageView> buildUnique();


        CombinedImageViewBuilder &asDefault();

        CombinedImageViewBuilder &asColor();

        CombinedImageViewBuilder &asDepth();
    };

} // vklite

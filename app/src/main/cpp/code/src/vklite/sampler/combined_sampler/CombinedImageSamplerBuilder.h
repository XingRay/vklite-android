//
// Created by leixing on 2025/6/6.
//

#pragma once

#include <functional>
#include <memory>

#include "vklite/sampler/combined_sampler/CombinedImageSampler.h"
#include "vklite/image/ImageBuilder.h"
#include "vklite/device_memory/DeviceMemoryBuilder.h"
#include "vklite/image_view/ImageViewBuilder.h"
#include "vklite/sampler/SamplerBuilder.h"

namespace vklite {

    class CombinedImageSamplerBuilder {
    private:
        vk::Device mDevice;

        ImageBuilder mImageBuilder;
        std::function<void(ImageBuilder &builder)> mImageBuilderConfigure;


        DeviceMemoryBuilder mDeviceMemoryBuilder;
        std::function<void(Image &image, DeviceMemoryBuilder &builder)> mDeviceMemoryBuilderConfigure;

        vk::DeviceSize mMemoryOffset;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;
        vk::MemoryPropertyFlags mMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;

        ImageViewBuilder mImageViewBuilder;
        std::function<void(Image &image, ImageViewBuilder &builder)> mImageViewBuilderConfigure;

        SamplerBuilder mSamplerBuilder;

    public:

        CombinedImageSamplerBuilder();

        ~CombinedImageSamplerBuilder();

        CombinedImageSamplerBuilder &device(vk::Device device);

        CombinedImageSamplerBuilder &configDeviceMemory(vk::PhysicalDevice physicalDevice);

        CombinedImageSamplerBuilder &configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        CombinedImageSamplerBuilder &width(uint32_t width);

        CombinedImageSamplerBuilder &height(uint32_t height);

        CombinedImageSamplerBuilder &format(vk::Format imageFormat);

        CombinedImageSamplerBuilder &sampleCount(vk::SampleCountFlagBits sampleCount);

        CombinedImageSampler build();

        std::unique_ptr<CombinedImageSampler> buildUnique();

        [[nodiscard]]
        std::vector<CombinedImageSampler> build(uint32_t count);
    };

} // vklite

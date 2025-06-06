//
// Created by leixing on 2025/6/6.
//

#include "CombinedImageSamplerBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    CombinedImageSamplerBuilder::CombinedImageSamplerBuilder()
            : mMemoryOffset(0) {
        mImageBuilder.asTextureImageBuilder();
        mImageViewBuilder.asDefault();
    }

    CombinedImageSamplerBuilder::~CombinedImageSamplerBuilder() = default;

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::device(vk::Device device) {
        mDevice = device;
        mImageBuilder.device(device);
        mDeviceMemoryBuilder.device(device);
        mImageViewBuilder.device(device);
        mSamplerBuilder.device(device);
        return *this;
    }

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        return *this;
    }

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::configDeviceMemory(vk::PhysicalDevice physicalDevice) {
        configDeviceMemory(physicalDevice.getMemoryProperties());
        return *this;
    }

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::width(uint32_t width) {
        mImageBuilder.width(width);
        return *this;
    }

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::height(uint32_t height) {
        mImageBuilder.height(height);
        return *this;
    }

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::format(vk::Format imageFormat) {
        mImageBuilder.format(imageFormat);
        mImageViewBuilder.format(imageFormat);
        return *this;
    }

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mImageBuilder.sampleCount(sampleCount);
        return *this;
    }

    CombinedImageSampler CombinedImageSamplerBuilder::build() {
        LOG_D("CombinedImageSamplerBuilder::build()");

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

        // create Sampler
        Sampler sampler = mSamplerBuilder.build();

        return {mDevice, mPhysicalDeviceMemoryProperties, std::move(image), std::move(deviceMemory), std::move(imageView), std::move(sampler)};
    }

    std::unique_ptr<CombinedImageSampler> CombinedImageSamplerBuilder::buildUnique() {
        return std::make_unique<CombinedImageSampler>(build());
    }

    std::vector<CombinedImageSampler> CombinedImageSamplerBuilder::build(uint32_t count) {
        std::vector<CombinedImageSampler> samplers;
        samplers.reserve(count);

        for (int i = 0; i < count; i++) {
            samplers.emplace_back(build());
        }

        return samplers;
    }

} // vklite
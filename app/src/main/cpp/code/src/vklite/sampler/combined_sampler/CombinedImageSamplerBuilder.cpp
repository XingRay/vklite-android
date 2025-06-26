//
// Created by leixing on 2025/6/6.
//

#include "CombinedImageSamplerBuilder.h"
#include "vklite/Log.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    CombinedImageSamplerBuilder::CombinedImageSamplerBuilder()
            : mMemoryOffset(0) {
        mImageBuilder.asTexture();
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

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::memoryOffset(vk::DeviceSize memoryOffset) {
        mMemoryOffset = memoryOffset;
        return *this;
    }

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        return *this;
    }

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::memoryPropertyFlags(vk::MemoryPropertyFlags memoryPropertyFlags) {
        mMemoryPropertyFlags = memoryPropertyFlags;
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

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::size(uint32_t width, uint32_t height) {
        mImageBuilder.size(width, height);
        return *this;
    }

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::size(vk::Extent2D size) {
        mImageBuilder.size(size);
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
        if (mDevice == nullptr) {
            throw std::runtime_error("CombinedImageSamplerBuilder::build(): mDevice == nullptr");
        }
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("CombinedImageSamplerBuilder::build(): mPhysicalDeviceMemoryProperties not set");
        }
        // create Image
        Image image = mImageBuilder.build();

        // create DeviceMemory
        vk::MemoryRequirements memoryRequirements = mDevice.getImageMemoryRequirements(image.getVkImage());
        mDeviceMemoryBuilder.allocationSize(memoryRequirements.size);

        uint32_t memoryTypeIndex = VulkanUtil::findMemoryTypeIndex(mPhysicalDeviceMemoryProperties.value(), memoryRequirements, mMemoryPropertyFlags);
        mDeviceMemoryBuilder.memoryTypeIndex(memoryTypeIndex);

        DeviceMemory deviceMemory = mDeviceMemoryBuilder.build();

        // bind memory
        image.bindMemory(deviceMemory.getVkDeviceMemory(), mMemoryOffset);

        // create ImageView
        mImageViewBuilder.image(image.getVkImage());
        ImageView imageView = mImageViewBuilder.build();

        // create Sampler
        Sampler sampler = mSamplerBuilder.build();

        return CombinedImageSampler{mDevice, mPhysicalDeviceMemoryProperties, std::move(image), std::move(deviceMemory), std::move(imageView), std::move(sampler)};
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

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::asDefault() {
        (*this)
                .memoryPropertyFlags(vk::MemoryPropertyFlagBits::eDeviceLocal);

        return *this;
    }

    CombinedImageSamplerBuilder &CombinedImageSamplerBuilder::asStorageImage() {
        asDefault();
        mImageBuilder.asStorageImage();
        mImageViewBuilder.asStorageImage();
        return *this;
    }

} // vklite
//
// Created by leixing on 2025/6/6.
//

#include "CombinedImageSampler.h"

#include "vklite/buffer/staging_buffer/StagingBufferBuilder.h"

namespace vklite {

    CombinedImageSampler::CombinedImageSampler(const vk::Device &device,
                                               std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties,
                                               Image &&image,
                                               DeviceMemory &&deviceMemory,
                                               ImageView &&imageView,
                                               Sampler &&sampler)
            : mDevice(device),
              mPhysicalDeviceMemoryProperties(physicalDeviceMemoryProperties),
              mImage(std::move(image)),
              mDeviceMemory(std::move(deviceMemory)),
              mImageView(std::move(imageView)),
              mSampler(std::move(sampler)) {}

    CombinedImageSampler::~CombinedImageSampler() = default;

    CombinedImageSampler::CombinedImageSampler(CombinedImageSampler &&other) noexcept
            : mDevice(other.mDevice),
              mPhysicalDeviceMemoryProperties(other.mPhysicalDeviceMemoryProperties),
              mImage(std::move(other.mImage)),
              mDeviceMemory(std::move(other.mDeviceMemory)),
              mImageView(std::move(other.mImageView)),
              mSampler(std::move(other.mSampler)) {}

    CombinedImageSampler &CombinedImageSampler::operator=(CombinedImageSampler &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mPhysicalDeviceMemoryProperties = std::exchange(other.mPhysicalDeviceMemoryProperties, std::nullopt);
            mImage = std::move(other.mImage);
            mDeviceMemory = std::move(other.mDeviceMemory);
            mImageView = std::move(other.mImageView);
            mSampler = std::move(other.mSampler);
        }
        return *this;
    }

    CombinedImageSampler &CombinedImageSampler::configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        return *this;
    }

    CombinedImageSampler &CombinedImageSampler::configDeviceMemory(vk::PhysicalDevice physicalDevice) {
        mPhysicalDeviceMemoryProperties = physicalDevice.getMemoryProperties();
        return *this;
    }

    Image &CombinedImageSampler::getImage() {
        return mImage;
    }

    const vk::Image &CombinedImageSampler::getVkImage() {
        return mImage.getVkImage();
    }

    DeviceMemory &CombinedImageSampler::getDeviceMemory() {
        return mDeviceMemory;
    }

    const vk::DeviceMemory &CombinedImageSampler::getVkDeviceMemory() {
        return mDeviceMemory.getVkDeviceMemory();
    }

    ImageView &CombinedImageSampler::getImageView() {
        return mImageView;
    }

    const vk::ImageView &CombinedImageSampler::getVkImageView() {
        return mImageView.getVkImageView();
    }

    Sampler &CombinedImageSampler::getSampler() {
        return mSampler;
    }

    const vk::Sampler &CombinedImageSampler::getVkSampler() {
        return mSampler.getVkSampler();
    }

//    update(){
//                vklite::StagingBuffer stagingBuffer = vklite::StagingBuffer(*mPhysicalDevice, *mDevice, textureImage->getPixelBytes());
//        stagingBuffer.updateBuffer(textureImage->getPixels(), textureImage->getPixelBytes());
//
//    }

    CombinedImageSampler &CombinedImageSampler::recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer) {
        mImage.recordCopyDataFromBuffer(commandBuffer, stagingBuffer);
        return *this;
    }

    CombinedImageSampler &CombinedImageSampler::recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer) {
        recordUpdate(commandBuffer, stagingBuffer.getVkBuffer());
        return *this;
    }


    CombinedImageSampler &CombinedImageSampler::update(const CommandPool &commandPool, const vk::Buffer &stagingBuffer) {
        commandPool.submit([&](const vk::CommandBuffer &commandBuffer) {
            recordUpdate(commandBuffer, stagingBuffer);
        });
        return *this;
    }

    CombinedImageSampler &CombinedImageSampler::update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer) {
        update(commandPool, stagingBuffer.getVkBuffer());
        return *this;
    }

    CombinedImageSampler &CombinedImageSampler::update(const CommandPool &commandPool, const void *data, uint32_t size) {
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("mPhysicalDeviceMemoryProperties not set, must invoke CombinedImageSampler::physicalDeviceMemoryProperties()");
        }
        StagingBuffer stagingBuffer = StagingBufferBuilder()
                .device(mDevice)
                .size(size)
                .physicalDeviceMemoryProperties(mPhysicalDeviceMemoryProperties.value())
                .build();
        stagingBuffer.updateBuffer(data, size);

        update(commandPool, stagingBuffer.getVkBuffer());
        return *this;
    }

} // vklite
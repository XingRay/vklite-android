//
// Created by leixing on 2025/3/13.
//

#include "Image.h"

#include <utility>

#include "vklite/buffer/staging_buffer/StagingBuffer.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/sync/pipeline_barrier/PipelineBarrierBuilder.h"

namespace vklite {

    Image::Image(const vk::Device &device, const vk::Image &image, const ImageMeta &meta)
            : mDevice(device), mImage(image), mMeta(meta) {}

    Image::~Image() {
        if (mDevice != nullptr && mImage != nullptr) {
            mDevice.destroy(mImage);
            mDevice = nullptr;
            mImage = nullptr;
        }
    }

    Image::Image(Image &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mImage(std::exchange(other.mImage, nullptr)),
              mMeta(other.mMeta) {}

    Image &Image::operator=(Image &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mImage = std::exchange(other.mImage, nullptr);
            mMeta = other.mMeta;
        }
        return *this;
    }

    const vk::Image &Image::getVkImage() const {
        return mImage;
    }

    uint32_t Image::getMipLevels() const {
        return mMeta.mipLevels;
    }

    vk::Format Image::getVkFormat() const {
        return mMeta.format;
    }


    uint32_t Image::getWidth() const {
        return mMeta.extent.width;
    }

    uint32_t Image::getHeight() const {
        return mMeta.extent.height;
    }

    Image &Image::bindMemory(vk::DeviceMemory deviceMemory, vk::DeviceSize memoryOffset) {
        mDevice.bindImageMemory(mImage, deviceMemory, memoryOffset);
        return *this;
    }

    Image &Image::copyDataFromBuffer(const CommandPool &commandPool, const vk::Buffer &buffer) {
        commandPool.submit([&](const vk::CommandBuffer &commandBuffer) {
            recordCopyDataFromBuffer(commandBuffer, buffer);
        });
        return *this;
    }

    Image &Image::recordCopyDataFromBuffer(const vk::CommandBuffer &commandBuffer, const vk::Buffer &buffer) {
        vk::ImageSubresourceLayers imageSubresourceLayers;
        imageSubresourceLayers
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setMipLevel(0)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        vk::Offset3D offset{0, 0, 0};
        vk::Extent3D extent{getWidth(), getHeight(), 1};

        vk::BufferImageCopy bufferImageCopy;
        bufferImageCopy
                .setBufferOffset(0)
                .setBufferRowLength(0)
                .setBufferImageHeight(0)
                .setImageSubresource(imageSubresourceLayers)
                .setImageOffset(offset)
                .setImageExtent(extent);

        std::array<vk::BufferImageCopy, 1> regions = {bufferImageCopy};

        commandBuffer.copyBufferToImage(buffer, mImage, vk::ImageLayout::eTransferDstOptimal, regions);

        return *this;
    }

    Image &Image::changeImageLayout(const vk::CommandBuffer &commandBuffer,
                                    vk::ImageLayout oldImageLayout,
                                    vk::ImageLayout newImageLayout,
                                    vk::PipelineStageFlags srcStage,
                                    vk::PipelineStageFlags dstStage,
                                    vk::AccessFlags srcAccessMask,
                                    vk::AccessFlags dstAccessMask,
                                    vk::ImageAspectFlags imageAspect) {
        PipelineBarrier pipelineBarrier = PipelineBarrierBuilder()
                .asDefault()
                .srcStage(srcStage)
                .dstStage(dstStage)
                .addImageMemoryBarrier([&](ImageMemoryBarrierBuilder &builder) {
                    builder
                            .asDefault()
                            .image(this->mImage)
                            .oldLayout(oldImageLayout)
                            .newLayout(newImageLayout)
                            .srcAccessMask(srcAccessMask)
                            .dstAccessMask(dstAccessMask)
                            .aspectMask(imageAspect);
                })
                .build();
        pipelineBarrier.record(commandBuffer);
        return *this;
    }

    Image &Image::changeImageLayout(const vk::CommandBuffer &commandBuffer) {
        changeImageLayout(commandBuffer,
                          vk::ImageLayout::eUndefined, vk::ImageLayout::eShaderReadOnlyOptimal,
                          vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eFragmentShader,
                          vk::AccessFlagBits::eNone, vk::AccessFlagBits::eShaderRead,
                          vk::ImageAspectFlagBits::eColor);
        return *this;
    }

    Image &Image::changeImageLayout(const CommandPool &commandPool,
                                    vk::ImageLayout oldImageLayout,
                                    vk::ImageLayout newImageLayout,
                                    vk::PipelineStageFlags srcStage,
                                    vk::PipelineStageFlags dstStage,
                                    vk::AccessFlags srcAccessMask,
                                    vk::AccessFlags dstAccessMask,
                                    vk::ImageAspectFlags imageAspect) {
        commandPool.submit([&](const vk::CommandBuffer &commandBuffer) {
            changeImageLayout(commandBuffer, oldImageLayout, newImageLayout, srcStage, dstStage, srcAccessMask, dstAccessMask, imageAspect);
        });
        return *this;
    }

    Image &Image::changeImageLayout(const CommandPool &commandPool) {
        commandPool.submit([&](const vk::CommandBuffer &commandBuffer) {
            changeImageLayout(commandBuffer);
        });
        return *this;
    }


    Image &Image::generateMipmaps(const CommandPool &commandPool) {
//        if (!mDevice.getPhysicalDevice().isSupportFormatFeature(mFormat, vk::FormatFeatureFlagBits::eSampledImageFilterLinear)) {
//            throw std::runtime_error("texture image format does not support linear tiling!");
//        }

        commandPool.submit([&](const vk::CommandBuffer &commandBuffer) {
            recordGenerateMipmaps(commandBuffer);
        });

        return *this;
    }

    Image &Image::recordGenerateMipmaps(const vk::CommandBuffer &commandBuffer) {

        vk::ImageSubresourceRange subresourceRange{};
        subresourceRange
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setLevelCount(1);

        vk::ImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier
                .setImage(mImage)
                .setSubresourceRange(subresourceRange)
                .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setDstQueueFamilyIndex(vk::QueueFamilyIgnored);

        int32_t mipWidth = static_cast<int32_t>(getWidth());
        int32_t mipHeight = static_cast<int32_t>(getHeight());
        uint32_t mipLevels = getMipLevels();

        for (int i = 1; i < mipLevels; i++) {
            subresourceRange.setBaseMipLevel(i - 1);

            imageMemoryBarrier
                    .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
                    .setNewLayout(vk::ImageLayout::eTransferSrcOptimal)
                    .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                    .setDstAccessMask(vk::AccessFlagBits::eTransferRead);

            commandBuffer.pipelineBarrier(
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::DependencyFlags{},
                    //memoryBarriers
                    {},
                    //bufferMemoryBarriers
                    {},
                    //imageMemoryBarriers
                    {imageMemoryBarrier}
            );

            std::array<vk::Offset3D, 2> srcOffsets{};
            srcOffsets[0] = vk::Offset3D{0, 0, 0};
            srcOffsets[1] = vk::Offset3D{mipWidth, mipHeight, 1};

            vk::ImageSubresourceLayers srcImageSubresourceLayers{};
            srcImageSubresourceLayers
                    .setAspectMask(vk::ImageAspectFlagBits::eColor)
                    .setMipLevel(i - 1)
                    .setBaseArrayLayer(0)
                    .setLayerCount(1);

            std::array<vk::Offset3D, 2> dstOffsets{};
            dstOffsets[0] = vk::Offset3D{0, 0, 0};
            dstOffsets[1] = vk::Offset3D{mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};

            vk::ImageSubresourceLayers dstImageSubresourceLayers{};
            dstImageSubresourceLayers
                    .setAspectMask(vk::ImageAspectFlagBits::eColor)
                    .setMipLevel(i)
                    .setBaseArrayLayer(0)
                    .setLayerCount(1);

            vk::ImageBlit imageBlit{};
            imageBlit
                    .setSrcOffsets(srcOffsets)
                    .setSrcSubresource(srcImageSubresourceLayers)
                    .setDstOffsets(dstOffsets)
                    .setDstSubresource(dstImageSubresourceLayers);

            std::array<vk::ImageBlit, 1> regions = {imageBlit};

            commandBuffer.blitImage(
                    mImage,
                    vk::ImageLayout::eTransferSrcOptimal,
                    mImage,
                    vk::ImageLayout::eTransferDstOptimal,
                    regions,
                    vk::Filter::eLinear
            );

            imageMemoryBarrier
                    .setOldLayout(vk::ImageLayout::eTransferSrcOptimal)
                    .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                    .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                    .setDstAccessMask(vk::AccessFlagBits::eShaderRead);

            commandBuffer.pipelineBarrier(
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::PipelineStageFlagBits::eFragmentShader,
                    vk::DependencyFlags{},
                    //memoryBarriers
                    {},
                    //bufferMemoryBarriers
                    {},
                    //imageMemoryBarriers
                    {imageMemoryBarrier}
            );

            if (mipWidth > 1) {
                mipWidth /= 2;
            }

            if (mipHeight > 1) {
                mipHeight /= 2;
            }
        }

        subresourceRange.setBaseMipLevel(getMipLevels() - 1);
        imageMemoryBarrier
                .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
                .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                .setDstAccessMask(vk::AccessFlagBits::eShaderRead);

        commandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eFragmentShader,
                vk::DependencyFlags{},
                //memoryBarriers
                {},
                //bufferMemoryBarriers
                {},
                //imageMemoryBarriers
                {imageMemoryBarrier}
        );

        return *this;
    }
} // vklite
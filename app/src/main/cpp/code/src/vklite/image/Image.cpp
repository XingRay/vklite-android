//
// Created by leixing on 2025/3/13.
//

#include "Image.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    Image::Image(const Device &device, uint32_t width, uint32_t height, vk::Format format, uint32_t mipLevels, vk::ImageUsageFlags imageUsageFlags,
                 vk::SampleCountFlagBits sampleCountFlagBits, vk::ImageTiling imageTiling)
            : mDevice(device), mWidth(width), mHeight(height), mMipLevels(mipLevels), mFormat(format) {

        const vk::Device &vkDevice = mDevice.getDevice();

        vk::Extent3D extent3D;
        extent3D
                .setWidth(width)
                .setHeight(height)
                .setDepth(1);

        vk::ImageCreateInfo imageCreateInfo;
        imageCreateInfo
                .setImageType(vk::ImageType::e2D)
                .setExtent(extent3D)
                .setMipLevels(mipLevels)
                .setArrayLayers(1)
                .setFormat(format)
                        //VK_IMAGE_TILING_LINEAR texel按行的顺序排列
                        //VK_IMAGE_TILING_OPTIMAL texel按实现定义的顺序排列
                        //the tiling mode cannot be changed at a later time.
                        //如果希望能够直接访问图像内存中的texel，则必须使用VK_IMAGE_TILING_LINEAR
                .setTiling(imageTiling)
                        //VK_IMAGE_LAYOUT_UNDEFINED 不能被 GPU 使用，并且第一个转换将丢弃texel
                        //VK_IMAGE_TILING_OPTIMAL 不能被 GPU 使用，并且第一个转换将保留texel
                .setInitialLayout(vk::ImageLayout::eUndefined)
//                .setUsage(vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
                .setUsage(imageUsageFlags)
                .setSharingMode(vk::SharingMode::eExclusive)
//                .setSamples(vk::SampleCountFlagBits::e1)
                .setSamples(sampleCountFlagBits)
                        //用于稀疏纹理相关的标志位
                .setFlags(vk::ImageCreateFlags{});

        mImage = vkDevice.createImage(imageCreateInfo);

        vk::MemoryRequirements memoryRequirements = vkDevice.getImageMemoryRequirements(mImage);
        uint32_t memoryTypeIndex = device.getPhysicalDevice().findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        vk::MemoryAllocateInfo memoryAllocateInfo;
        memoryAllocateInfo
                .setAllocationSize(memoryRequirements.size)
                .setMemoryTypeIndex(memoryTypeIndex);

        mDeviceMemory = vkDevice.allocateMemory(memoryAllocateInfo);

        vkDevice.bindImageMemory(mImage, mDeviceMemory, 0);
    }

    Image::~Image() {
        const vk::Device &vkDevice = mDevice.getDevice();

//        device.destroy(mImageView);
        vkDevice.destroy(mImage);
        vkDevice.unmapMemory(mDeviceMemory);
    }

    const vk::Image &Image::getImage() const {
        return mImage;
    }

//    const vk::ImageView &Image::getImageView() const {
//        return mImageView;
//    }

    uint32_t Image::getMipLevels() const {
        return mMipLevels;
    }

    const vk::DeviceMemory &Image::getDeviceMemory() const {
        return mDeviceMemory;
    }

    vk::Format Image::getFormat() const {
        return mFormat;
    }


    uint32_t Image::getWidth() const {
        return mWidth;
    }

    uint32_t Image::getHeight() const {
        return mHeight;
    }

    void Image::copyDataFromBuffer(const CommandPool &commandPool, const vk::Buffer &buffer) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyDataFromBuffer(commandBuffer, buffer);
        });
    }

    void Image::recordCommandCopyDataFromBuffer(const vk::CommandBuffer &commandBuffer, const vk::Buffer &buffer) {
        vk::ImageSubresourceLayers imageSubresourceLayers;
        imageSubresourceLayers
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setMipLevel(0)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        vk::Offset3D offset{0, 0, 0};
        vk::Extent3D extent{mWidth, mHeight, 1};

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
    }

    void Image::transitionImageLayout(const CommandPool &commandPool,
                                      vk::ImageLayout oldImageLayout,
                                      vk::ImageLayout newImageLayout,
                                      uint32_t levelCount,
                                      uint32_t srcQueueFamilyIndex,
                                      uint32_t dstQueueFamilyIndex,
                                      vk::ImageAspectFlags imageAspectFlags) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandTransitionImageLayout(commandBuffer, oldImageLayout, newImageLayout, levelCount, srcQueueFamilyIndex, dstQueueFamilyIndex, imageAspectFlags);
        });
    }

    void Image::recordCommandTransitionImageLayout(const vk::CommandBuffer &commandBuffer,
                                                   vk::ImageLayout oldImageLayout,
                                                   vk::ImageLayout newImageLayout,
                                                   uint32_t levelCount,
                                                   uint32_t srcQueueFamilyIndex,
                                                   uint32_t dstQueueFamilyIndex,
                                                   vk::ImageAspectFlags imageAspectFlags) {

        vk::ImageSubresourceRange imageSubresourceRange;
        imageSubresourceRange
                .setBaseMipLevel(0)
                .setLevelCount(levelCount)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setAspectMask(imageAspectFlags);

        vk::ImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier
//                .setOldLayout(vk::ImageLayout::eUndefined)
//                .setNewLayout(vk::ImageLayout::eTransferDstOptimal)
                .setOldLayout(oldImageLayout)
                .setNewLayout(newImageLayout)
//                .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setSrcQueueFamilyIndex(srcQueueFamilyIndex)
//                .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setDstQueueFamilyIndex(dstQueueFamilyIndex)
                .setImage(mImage)
                .setSubresourceRange(imageSubresourceRange)
                .setSrcAccessMask(vk::AccessFlagBits::eNone)
                .setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite);

        // 内存屏障
        std::vector<vk::MemoryBarrier> memoryBarriers = {};

        // 缓冲区内存屏障
        std::vector<vk::BufferMemoryBarrier> bufferMemoryBarriers = {};

        // 图像内存屏障
        std::array<vk::ImageMemoryBarrier, 1> imageMemoryBarriers = {imageMemoryBarrier};

        commandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eTopOfPipe,
//                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eEarlyFragmentTests,
                vk::DependencyFlags{},
                memoryBarriers,
                bufferMemoryBarriers,
                imageMemoryBarriers
        );
    }

    void Image::generateMipmaps(const CommandPool &commandPool) {
        if (!mDevice.getPhysicalDevice().isSupportFormatFeature(mFormat, vk::FormatFeatureFlagBits::eSampledImageFilterLinear)) {
            throw std::runtime_error("texture image format does not support linear tiling!");
        }

        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandGenerateMipmaps(commandBuffer);
        });
    }

    void Image::recordCommandGenerateMipmaps(const vk::CommandBuffer &commandBuffer) {

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

        int32_t mipWidth = static_cast<int32_t>(mWidth);
        int32_t mipHeight = static_cast<int32_t>(mHeight);

        for (int i = 1; i < mMipLevels; i++) {
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

        subresourceRange.setBaseMipLevel(mMipLevels - 1);
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

    }
} // vklite
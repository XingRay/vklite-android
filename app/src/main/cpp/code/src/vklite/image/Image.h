//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/command_buffer/CommandPool.h"
#include "vklite/buffer/host_visible/StagingBuffer.h"
#include "vklite/image/ImageInterface.h"

namespace vklite {

    class Image : public ImageInterface {
    private:
        const Device &mDevice;
        vk::Format mFormat;
        uint32_t mMipLevels;
        uint32_t mWidth;
        uint32_t mHeight;

        // TransitionImageLayout params
//        vk::ImageLayout mOldImageLayout;
//        vk::ImageLayout mNewImageLayout;
//        uint32_t mSrcQueueFamilyIndex;
//        uint32_t mDstQueueFamilyIndex;
//        vk::ImageAspectFlags mImageAspectFlags;

        vk::Image mImage;
        vk::DeviceMemory mDeviceMemory;
//        vk::ImageView mImageView;

//        std::unique_ptr<VulkanStagingBuffer> mStagingBuffer;

    public:
        Image(const Device &device, uint32_t width, uint32_t height, vk::Format format, uint32_t mipLevels, vk::ImageUsageFlags imageUsageFlags,
              vk::SampleCountFlagBits sampleCountFlagBits, vk::ImageTiling imageTiling);

        ~Image() override;

        [[nodiscard]]
        const vk::Image &getImage() const override;

//        [[nodiscard]]
//        const vk::ImageView &getImageView() const override;

        [[nodiscard]]
        uint32_t getMipLevels() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        vk::Format getFormat() const;

        [[nodiscard]]
        uint32_t getWidth() const;

        [[nodiscard]]
        uint32_t getHeight() const;


        // copy data from buffer
        void copyDataFromBuffer(const CommandPool &commandPool, const vk::Buffer &buffer);

        void recordCommandCopyDataFromBuffer(const vk::CommandBuffer &commandBuffer, const vk::Buffer &buffer);


        // generate mipmaps
        void generateMipmaps(const CommandPool &commandPool);

        void recordCommandGenerateMipmaps(const vk::CommandBuffer &commandBuffer);


        // transition image layout
        void transitionImageLayout(const CommandPool &commandPool,
                                   vk::ImageLayout oldImageLayout,
                                   vk::ImageLayout newImageLayout,
                                   uint32_t levelCount,
                                   uint32_t srcQueueFamilyIndex,
                                   uint32_t dstQueueFamilyIndex,
                                   vk::ImageAspectFlags imageAspectFlags);

        void recordCommandTransitionImageLayout(const vk::CommandBuffer &commandBuffer,
                                                vk::ImageLayout oldImageLayout,
                                                vk::ImageLayout newImageLayout,
                                                uint32_t levelCount,
                                                uint32_t srcQueueFamilyIndex,
                                                uint32_t dstQueueFamilyIndex,
                                                vk::ImageAspectFlags imageAspectFlags);


    };

} // vklite

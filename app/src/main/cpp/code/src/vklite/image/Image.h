//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/command_buffer/CommandPool.h"
#include "vklite/buffer/host_visible/StagingBuffer.h"
#include "vklite/image/ImageInterface.h"
#include "vklite/image/ImageTransition.h"

namespace vklite {

    class Image : public ImageInterface {
    private:
        const Device &mDevice;

        vk::Format mFormat;
        uint32_t mMipLevels;
        uint32_t mWidth;
        uint32_t mHeight;

        vk::Image mImage;
        vk::DeviceMemory mDeviceMemory;

    public:
        Image(const Device &device, const vk::ImageCreateInfo &imageCreateInfo);

        ~Image() override;

        Image(const Image &other) = delete;

        Image &operator=(const Image &other) = delete;

        Image(Image &&other) noexcept;

        Image &operator=(Image &&other) noexcept = delete;

        [[nodiscard]]
        const vk::Image &getImage() const override;

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
        Image &copyDataFromBuffer(const CommandPool &commandPool, const vk::Buffer &buffer);

        Image &recordCopyDataFromBuffer(const vk::CommandBuffer &commandBuffer, const vk::Buffer &buffer);


        // generate mipmaps
        Image &generateMipmaps(const CommandPool &commandPool);

        Image &recordGenerateMipmaps(const vk::CommandBuffer &commandBuffer);


        // transition image layout
        Image &transitionImageLayout(const CommandPool &commandPool,
                                     vk::ImageLayout oldImageLayout,
                                     vk::ImageLayout newImageLayout,
                                     uint32_t levelCount,
                                     uint32_t srcQueueFamilyIndex,
                                     uint32_t dstQueueFamilyIndex,
                                     vk::ImageAspectFlags aspectMask);

        Image &transitionImageLayout(const CommandPool &commandPool, const ImageTransition &imageTransition);

        Image &recordTransitionImageLayout(const vk::CommandBuffer &commandBuffer,
                                           vk::ImageLayout oldImageLayout,
                                           vk::ImageLayout newImageLayout,
                                           uint32_t levelCount,
                                           uint32_t srcQueueFamilyIndex,
                                           uint32_t dstQueueFamilyIndex,
                                           vk::ImageAspectFlags aspectMask);

        Image &recordTransitionImageLayout(const vk::CommandBuffer &commandBuffer, const ImageTransition &imageTransition);
    };

} // vklite

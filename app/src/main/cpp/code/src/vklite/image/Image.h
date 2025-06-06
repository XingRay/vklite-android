//
// Created by leixing on 2025/3/13.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include "vklite/device/Device.h"
#include "vklite/command_pool/CommandPool.h"
#include "vklite/buffer/staging_buffer/StagingBuffer.h"
#include "vklite/image/ImageTransition.h"
#include "vklite/image/ImageMeta.h"

namespace vklite {

    class Image {
    private:
        vk::Device mDevice;
        vk::Image mImage;
        ImageMeta mMeta;

    public:
        Image(const vk::Device &device, const vk::Image &image, const ImageMeta &meta);

        ~Image();

        Image(const Image &other) = delete;

        Image &operator=(const Image &other) = delete;

        Image(Image &&other) noexcept;

        Image &operator=(Image &&other) noexcept;

        [[nodiscard]]
        const vk::Image &getImage() const;

        [[nodiscard]]
        uint32_t getMipLevels() const;

        [[nodiscard]]
        vk::Format getFormat() const;

        [[nodiscard]]
        uint32_t getWidth() const;

        [[nodiscard]]
        uint32_t getHeight() const;

        Image &bindMemory(vk::DeviceMemory deviceMemory, vk::DeviceSize memoryOffset = 0);

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

        Image &transitionImageLayout(const CommandPool &commandPool);

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

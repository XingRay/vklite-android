//
// Created by leixing on 2025/3/13.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include "vklite/command_pool/CommandPool.h"
#include "vklite/image/ImageMeta.h"
#include "vklite/sync/pipeline_barrier/PipelineBarrier.h"

namespace vklite {

    class Image {
    private:
        vk::Device mDevice;
        vk::Image mImage;
        ImageMeta mMeta;

    public:
        explicit Image(const vk::Device &device, const vk::Image &image, const ImageMeta &meta);

        ~Image();

        Image(const Image &other) = delete;

        Image &operator=(const Image &other) = delete;

        Image(Image &&other) noexcept;

        Image &operator=(Image &&other) noexcept;

        [[nodiscard]]
        const vk::Image &getVkImage() const;

        [[nodiscard]]
        uint32_t getMipLevels() const;

        [[nodiscard]]
        vk::Format getVkFormat() const;

        [[nodiscard]]
        uint32_t getWidth() const;

        [[nodiscard]]
        uint32_t getHeight() const;

        Image &bindMemory(vk::DeviceMemory deviceMemory, vk::DeviceSize memoryOffset = 0);

        // copy data from buffer
        Image &copyDataFromBuffer(const CommandPool &commandPool, const vk::Buffer &buffer);

        Image &recordCopyDataFromBuffer(const vk::CommandBuffer &commandBuffer, const vk::Buffer &buffer);


        // changeImageLayout
        Image &changeImageLayout(const vk::CommandBuffer &commandBuffer,
                                 vk::ImageLayout oldImageLayout,
                                 vk::ImageLayout newImageLayout,
                                 vk::PipelineStageFlags srcStage,
                                 vk::PipelineStageFlags dstStage,
                                 vk::AccessFlags srcAccessMask,
                                 vk::AccessFlags dstAccessMask,
                                 vk::ImageAspectFlags imageAspect);

        Image &changeImageLayout(const vk::CommandBuffer &commandBuffer);

        Image &changeImageLayout(const CommandPool &commandPool,
                                 vk::ImageLayout oldImageLayout,
                                 vk::ImageLayout newImageLayout,
                                 vk::PipelineStageFlags srcStage,
                                 vk::PipelineStageFlags dstStage,
                                 vk::AccessFlags srcAccessMask,
                                 vk::AccessFlags dstAccessMask,
                                 vk::ImageAspectFlags imageAspect);

        Image &changeImageLayout(const CommandPool &commandPool);


        // generate mipmaps
        Image &generateMipmaps(const CommandPool &commandPool);

        Image &recordGenerateMipmaps(const vk::CommandBuffer &commandBuffer);
    };

} // vklite

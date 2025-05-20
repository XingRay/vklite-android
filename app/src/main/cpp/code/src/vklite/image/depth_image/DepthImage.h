//
// Created by leixing on 2025/5/19.
//

#pragma once

#include "vklite/device/Device.h"
#include "vklite/image/ImageInterface.h"
#include "vklite/image/Image.h"

namespace vklite {

    class DepthImage : public ImageInterface {
    private:
        Image mImage;

    public:
        explicit DepthImage(Image &&image);

        ~DepthImage() override;

        DepthImage(const DepthImage &other) = delete;

        DepthImage &operator=(const DepthImage &other) = delete;

        DepthImage(DepthImage &&other) noexcept;

        DepthImage &operator=(DepthImage &&other) noexcept = delete;

        [[nodiscard]]
        const vk::Image &getImage() const override;

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

        void transitionImageLayout(const CommandPool &commandPool);

        void recordCommandTransitionImageLayout(const vk::CommandBuffer &commandBuffer);
    };

} // vklite

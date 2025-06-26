//
// Created by leixing on 2025/6/6.
//

#pragma once

#include "vklite/image/Image.h"
#include "vklite/device_memory/DeviceMemory.h"
#include "vklite/image_view/ImageView.h"
#include "vklite/sampler/Sampler.h"
#include "vklite/buffer/staging_buffer/StagingBuffer.h"

namespace vklite {

    class CombinedImageSampler {
        vk::Device mDevice;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

        Image mImage;
        DeviceMemory mDeviceMemory;
        ImageView mImageView;
        Sampler mSampler;

    public:
        explicit CombinedImageSampler(const vk::Device &device,
                                      std::optional<vk::PhysicalDeviceMemoryProperties> physicalDeviceMemoryProperties,
                                      Image &&image,
                                      DeviceMemory &&deviceMemory,
                                      ImageView &&imageView,
                                      Sampler &&sampler);

        ~CombinedImageSampler();

        CombinedImageSampler(const CombinedImageSampler &other) = delete;

        CombinedImageSampler &operator=(const CombinedImageSampler &other) = delete;

        CombinedImageSampler(CombinedImageSampler &&other) noexcept;

        CombinedImageSampler &operator=(CombinedImageSampler &&other) noexcept;

        CombinedImageSampler &configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        CombinedImageSampler &configDeviceMemory(vk::PhysicalDevice physicalDevice);

        [[nodiscard]]
        Image &getImage();

        [[nodiscard]]
        const vk::Image &getVkImage();

        [[nodiscard]]
        DeviceMemory &getDeviceMemory();

        [[nodiscard]]
        const vk::DeviceMemory &getVkDeviceMemory();

        [[nodiscard]]
        ImageView &getImageView();

        [[nodiscard]]
        const vk::ImageView &getVkImageView();

        [[nodiscard]]
        Sampler &getSampler();

        [[nodiscard]]
        const vk::Sampler &getVkSampler();

        CombinedImageSampler &recordUpdate(const vk::CommandBuffer &commandBuffer, vk::Buffer stagingBuffer);

        CombinedImageSampler &recordUpdate(const vk::CommandBuffer &commandBuffer, const StagingBuffer &stagingBuffer);


        CombinedImageSampler &update(const CommandPool &commandPool, const vk::Buffer &stagingBuffer);

        CombinedImageSampler &update(const CommandPool &commandPool, const StagingBuffer &stagingBuffer);

        CombinedImageSampler &update(const CommandPool &commandPool, const void *data, uint32_t size);
    };

} // vklite

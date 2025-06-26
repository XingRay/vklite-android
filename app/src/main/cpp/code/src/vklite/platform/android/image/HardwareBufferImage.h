//
// Created by leixing on 2025/6/9.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/platform/android/image/HardwareBufferImageMeta.h"

namespace vklite {

    class HardwareBufferImage {
    private:
        vk::Device mDevice;
        vk::Image mImage;
        HardwareBufferImageMeta mMeta;

    public:
        explicit HardwareBufferImage(const vk::Device &device, const vk::Image &image, const HardwareBufferImageMeta &meta);

        ~HardwareBufferImage();

        HardwareBufferImage(const HardwareBufferImage &other) = delete;

        HardwareBufferImage &operator=(const HardwareBufferImage &other) = delete;

        HardwareBufferImage(HardwareBufferImage &&other) noexcept;

        HardwareBufferImage &operator=(HardwareBufferImage &&other) noexcept;

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

        HardwareBufferImage &bindMemory(vk::DeviceMemory deviceMemory, vk::DeviceSize memoryOffset = 0);
    };

} // vklite

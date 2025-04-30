//
// Created by leixing on 2025/1/19.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <android/hardware_buffer.h>

#include "vklite/instance/Instance.h"
#include "vklite/device/Device.h"
#include "vklite/command/CommandPool.h"
#include "vklite/buffer/host_visible/VulkanHostVisibleBuffer.h"
#include "vklite/image/VulkanImage.h"

#include "AndroidHardwareBuffer.h"
#include "VulkanAndroidHardwareBufferYcbcrConversion.h"
#include "vklite/image/VulkanImageInterface.h"

namespace vklite {

    class VulkanAndroidHardwareBufferImage : public VulkanImageInterface {
    private:
        const Device &mDevice;

        vk::Image mImage;
        vk::DeviceMemory mDeviceMemory;
        vk::ImageView mImageView;

    public:
        VulkanAndroidHardwareBufferImage(const Device &device,
                                         const AndroidHardwareBuffer &androidHardwareBuffer,
                                         const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion);

        ~VulkanAndroidHardwareBufferImage() override;

        [[nodiscard]]
        const vk::Image &getImage() const override;

        [[nodiscard]]
        const vk::ImageView &getImageView() const override;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        uint32_t getMipLevels() const override;

        void transitionImageLayout(const CommandPool &commandPool) override;

        void update(const CommandPool &vulkanCommandPool, const void *data, uint32_t size) override;

    };

} // vklite

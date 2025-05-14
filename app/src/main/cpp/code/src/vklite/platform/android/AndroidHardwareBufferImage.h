//
// Created by leixing on 2025/1/19.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <android/hardware_buffer.h>

#include "vklite/instance/Instance.h"
#include "vklite/device/Device.h"
#include "vklite/command/CommandPool.h"
#include "vklite/buffer/host_visible/HostVisibleBuffer.h"
#include "vklite/image/Image.h"

#include "AndroidHardwareBuffer.h"
#include "AndroidHardwareBufferYcbcrConversion.h"
#include "vklite/image/ImageInterface.h"

namespace vklite {

    class AndroidHardwareBufferImage : public ImageInterface {
    private:
        const Device &mDevice;

        vk::Image mImage;
        vk::DeviceMemory mDeviceMemory;
        vk::ImageView mImageView;

    public:
        AndroidHardwareBufferImage(const Device &device,
                                   const AndroidHardwareBuffer &androidHardwareBuffer,
                                   const AndroidHardwareBufferYcbcrConversion &conversion);

        ~AndroidHardwareBufferImage() override;

        [[nodiscard]]
        const vk::Image &getImage() const override;

//        [[nodiscard]]
//        const vk::ImageView &getImageView() const override;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        vk::Format getFormat() const override;

        [[nodiscard]]
        uint32_t getMipLevels() const override;

        void transitionImageLayout(const CommandPool &commandPool) override;

        void update(const CommandPool &commandPool, const void *data, uint32_t size) override;

    };

} // vklite

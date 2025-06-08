//
// Created by leixing on 2025/1/19.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <android/hardware_buffer.h>

#include "vklite/instance/Instance.h"
#include "vklite/device/Device.h"
#include "vklite/command_pool/CommandPool.h"
#include "vklite/buffer/host_visible/HostVisibleBuffer.h"
#include "vklite/image/Image.h"

#include "vklite/platform/android/hardware_buffer/HardwareBuffer.h"
#include "vklite/platform/android/conversion/HardwareBufferYcbcrConversion.h"

namespace vklite {

//    class HardwareBufferImage {
//    private:
//        const Device &mDevice;
//
//        vk::Image mImage;
//        vk::DeviceMemory mDeviceMemory;
//        vk::ImageView mImageView;
//
//    public:
//        HardwareBufferImage(const PhysicalDevice &physicalDevice,
//                            const Device &device,
//                            const HardwareBuffer &androidHardwareBuffer,
//                            const HardwareBufferYcbcrConversion &conversion);
//
//        ~HardwareBufferImage();
//
//        [[nodiscard]]
//        const vk::Image &getImage() const;
//
//        [[nodiscard]]
//        const vk::DeviceMemory &getDeviceMemory() const;
//
//    };

} // vklite

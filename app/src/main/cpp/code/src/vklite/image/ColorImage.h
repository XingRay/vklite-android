//
// Created by leixing on 2025/5/6.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "vklite/device/Device.h"
#include "vklite/image/ImageInterface.h"
#include "vklite/command/CommandPool.h"

namespace vklite {

    class ColorImage{
    private:
        const Device &mDevice;

        vk::Image mImage;
        vk::DeviceMemory mDeviceMemory;

    public:
        ColorImage(const Device &device, uint32_t width, uint32_t height, vk::SampleCountFlagBits sampleCountFlagBits, vk::Format displayFormat);

        ~ColorImage();
    };

} // vklite

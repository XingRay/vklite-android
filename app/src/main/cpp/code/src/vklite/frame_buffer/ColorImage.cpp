//
// Created by leixing on 2025/5/6.
//

#include "ColorImage.h"

namespace vklite {

    ColorImage::ColorImage(const Device &device, uint32_t width, uint32_t height, vk::SampleCountFlagBits sampleCountFlagBits, vk::Format displayFormat)
            : mDevice(device) {
        std::tie(mImage, mDeviceMemory) = device.createImage(width, height, 1,
                                                             sampleCountFlagBits,
                                                             displayFormat,
                                                             vk::ImageTiling::eOptimal,
                                                             vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
                                                             vk::MemoryPropertyFlagBits::eDeviceLocal);
        mImageView = device.createImageView(mImage, displayFormat, vk::ImageAspectFlagBits::eColor, 1);
    }

    ColorImage::~ColorImage() {
        const vk::Device &vkDevice = mDevice.getDevice();
        vkDevice.destroy(mImageView);
        vkDevice.destroy(mImage);
        vkDevice.freeMemory(mDeviceMemory);
    }

} // vklite
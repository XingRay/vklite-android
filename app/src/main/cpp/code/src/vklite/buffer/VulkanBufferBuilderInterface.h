//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "vklite/device/Device.h"
#include "VulkanBufferInterface.h"

namespace vklite {

    class VulkanBufferBuilderInterface {
    public:
        VulkanBufferBuilderInterface();

        virtual ~VulkanBufferBuilderInterface() = 0;

        virtual std::unique_ptr<VulkanBufferInterface> build(const Device &vulkanDevice, vk::DeviceSize bufferSize) = 0;
    };

} // vklite

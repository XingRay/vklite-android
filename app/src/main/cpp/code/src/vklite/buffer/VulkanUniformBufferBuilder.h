//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "vklite/buffer/device_local/VulkanDeviceLocalUniformBuffer.h"
#include "VulkanBufferBuilderInterface.h"

namespace vklite {

    class VulkanUniformBufferBuilder : public VulkanBufferBuilderInterface {
    public:

        VulkanUniformBufferBuilder();

        ~VulkanUniformBufferBuilder() override;

        std::unique_ptr<VulkanBufferInterface> build(const Device &device, vk::DeviceSize bufferSize) override;
    };

} // vklite

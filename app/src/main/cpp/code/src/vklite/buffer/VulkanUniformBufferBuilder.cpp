//
// Created by leixing on 2025/4/4.
//

#include "VulkanUniformBufferBuilder.h"

namespace vklite {

    VulkanUniformBufferBuilder::VulkanUniformBufferBuilder() = default;

    VulkanUniformBufferBuilder::~VulkanUniformBufferBuilder() = default;

    std::unique_ptr<VulkanBufferInterface> VulkanUniformBufferBuilder::build(const Device &device, vk::DeviceSize bufferSize) {
        VulkanDeviceLocalUniformBuffer buffer(device, bufferSize);
        return std::make_unique<VulkanDeviceLocalUniformBuffer>(device, bufferSize);
    }

} // vklite
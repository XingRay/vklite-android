//
// Created by leixing on 2025/4/4.
//

#include "UniformBufferBuilder.h"

namespace vklite {

    UniformBufferBuilder::UniformBufferBuilder() = default;

    UniformBufferBuilder::~UniformBufferBuilder() = default;

    std::unique_ptr<BufferInterface> UniformBufferBuilder::build(const PhysicalDevice &physicalDevice, const Device &device, vk::DeviceSize bufferSize) {
        return std::make_unique<UniformBuffer>(physicalDevice, device, bufferSize);
    }

} // vklite
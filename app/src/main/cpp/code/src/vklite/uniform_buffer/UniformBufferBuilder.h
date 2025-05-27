//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "UniformBuffer.h"
#include "vklite/buffer/BufferBuilderInterface.h"

namespace vklite {

    class UniformBufferBuilder : public BufferBuilderInterface {
    public:

        UniformBufferBuilder();

        ~UniformBufferBuilder() override;

        std::unique_ptr<BufferInterface> build(const PhysicalDevice &physicalDevice, const Device &device, vk::DeviceSize bufferSize) override;
    };

} // vklite

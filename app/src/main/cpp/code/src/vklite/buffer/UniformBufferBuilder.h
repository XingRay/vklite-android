//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "vklite/buffer/device_local/DeviceLocalUniformBuffer.h"
#include "BufferBuilderInterface.h"

namespace vklite {

    class UniformBufferBuilder : public BufferBuilderInterface {
    public:

        UniformBufferBuilder();

        ~UniformBufferBuilder() override;

        std::unique_ptr<BufferInterface> build(const Device &device, vk::DeviceSize bufferSize) override;
    };

} // vklite

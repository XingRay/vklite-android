//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>

#include "VertexBuffer.h"

namespace vklite {

    class VertexBufferBuilder {
    private:
        vk::DeviceSize mBufferSize;

    public:
        VertexBufferBuilder();

        ~VertexBufferBuilder();

        VertexBufferBuilder &bufferSize(vk::DeviceSize bufferSize);

        std::unique_ptr<VertexBuffer> build(const PhysicalDevice &physicalDevice, const Device &device);
    };

} // vklite

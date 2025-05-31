//
// Created by leixing on 2025/5/1.
//

#include "VertexBufferBuilder.h"

namespace vklite {

    VertexBufferBuilder::VertexBufferBuilder()
            : mBufferSize(0) {}

    VertexBufferBuilder::~VertexBufferBuilder() = default;

    VertexBufferBuilder &VertexBufferBuilder::bufferSize(vk::DeviceSize bufferSize) {
        mBufferSize = bufferSize;
        return *this;
    }

    std::unique_ptr<VertexBuffer> VertexBufferBuilder::build(const PhysicalDevice &physicalDevice, const Device &device) {
        return std::make_unique<VertexBuffer>(physicalDevice, device, mBufferSize);
    }

} // vklite
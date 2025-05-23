//
// Created by leixing on 2025/3/22.
//

#include "IndexBufferBuilder.h"

namespace vklite {

    IndexBufferBuilder::IndexBufferBuilder()
            : mBufferSize(0), mIndexType(vk::IndexType::eUint32) {}

    IndexBufferBuilder::~IndexBufferBuilder() = default;

    IndexBufferBuilder &IndexBufferBuilder::bufferSize(vk::DeviceSize bufferSize) {
        mBufferSize = bufferSize;
        return *this;
    }

    IndexBufferBuilder &IndexBufferBuilder::indexType(vk::IndexType indexType) {
        mIndexType = indexType;
        return *this;
    }

    std::unique_ptr<IndexBuffer> IndexBufferBuilder::build(const Device &device) const {
        return std::make_unique<IndexBuffer>(device, mBufferSize);
    }

} // vklite
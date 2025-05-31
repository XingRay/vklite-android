//
// Created by leixing on 2025/3/22.
//

#include "IndexBufferBuilder.h"
#include "vklite/buffer/index_buffer/IndexBufferMeta.h"

namespace vklite {

    IndexBufferBuilder::IndexBufferBuilder()
            : mIndexType(vk::IndexType::eUint32) {
        mCombinedMemoryBufferBuilder.asDeviceLocal().addUsage(vk::BufferUsageFlagBits::eIndexBuffer);
    }

    IndexBufferBuilder::~IndexBufferBuilder() = default;

    IndexBufferBuilder &IndexBufferBuilder::device(vk::Device device) {
        mDevice  = device;
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    IndexBufferBuilder &IndexBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    IndexBufferBuilder &IndexBufferBuilder::indexType(vk::IndexType indexType) {
        mIndexType = indexType;
        return *this;
    }

    IndexBuffer IndexBufferBuilder::build() {
        CombinedMemoryBuffer buffer = mCombinedMemoryBufferBuilder.build();
        IndexBufferMeta meta(mIndexType, 0);
        return {mDevice, std::move(buffer), std::move(meta)};
    }

    std::unique_ptr<IndexBuffer> IndexBufferBuilder::buildUnique() {
        return std::make_unique<IndexBuffer>(build());
    }

} // vklite
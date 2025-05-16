//
// Created by leixing on 2025/5/16.
//

#include "IndexBufferInfo.h"

namespace vklite {

    IndexBufferInfo::IndexBufferInfo(const IndexBuffer &indexBuffer, vk::DeviceSize offset)
            : mIndexBuffer(indexBuffer), mOffset(offset) {}

    IndexBufferInfo::IndexBufferInfo(const IndexBufferInfo &other) = default;

    IndexBufferInfo::IndexBufferInfo(IndexBufferInfo &&other)
            : mIndexBuffer(other.mIndexBuffer), mOffset(other.mOffset) {}

    IndexBufferInfo::~IndexBufferInfo() = default;

    const IndexBuffer &IndexBufferInfo::getIndexBuffer() const {
        return mIndexBuffer;
    }

    vk::DeviceSize IndexBufferInfo::getOffset() const {
        return mOffset;
    }

} // vklite
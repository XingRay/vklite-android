//
// Created by leixing on 2025/5/16.
//

#include "VertexBufferInfo.h"

namespace vklite {

    VertexBufferInfo::VertexBufferInfo(const VertexBuffer &vertexBuffer, vk::DeviceSize offset)
            : mVertexBuffer(vertexBuffer), mOffset(offset) {}

    VertexBufferInfo::VertexBufferInfo(const VertexBufferInfo &other)=default;

    VertexBufferInfo::VertexBufferInfo(VertexBufferInfo &&other) noexcept
            : mVertexBuffer(other.mVertexBuffer), mOffset(other.mOffset) {}

    VertexBufferInfo::~VertexBufferInfo() = default;

    const VertexBuffer &VertexBufferInfo::getVertexBuffer() const {
        return mVertexBuffer;
    }

    vk::DeviceSize VertexBufferInfo::getOffset() const {
        return mOffset;
    }

} // vklite
//
// Created by leixing on 2025/6/1.
//

#include "IndexBufferMeta.h"

namespace vklite {

    IndexBufferMeta::IndexBufferMeta(vk::IndexType indexType, uint32_t indicesCount)
            : mIndexType(indexType), mIndicesCount(indicesCount) {}

    IndexBufferMeta::~IndexBufferMeta() = default;

    IndexBufferMeta::IndexBufferMeta(const IndexBufferMeta &other) = default;

    IndexBufferMeta &IndexBufferMeta::operator=(const IndexBufferMeta &other) {
        if (this != &other) {
            mIndexType = other.mIndexType;
            mIndicesCount = other.mIndicesCount;
        }
        return *this;
    }

    IndexBufferMeta::IndexBufferMeta(IndexBufferMeta &&other) noexcept
            : mIndexType(other.mIndexType),
              mIndicesCount(other.mIndicesCount) {}

    IndexBufferMeta &IndexBufferMeta::operator=(IndexBufferMeta &&other) noexcept {
        if (this != &other) {
            mIndexType = other.mIndexType;
            mIndicesCount = other.mIndicesCount;
        }
        return *this;
    }

    IndexBufferMeta &IndexBufferMeta::indicesCount(uint32_t indicesCount) {
        mIndicesCount = indicesCount;
        return *this;
    }

    vk::IndexType IndexBufferMeta::getIndexType() const {
        return mIndexType;
    }

    uint32_t IndexBufferMeta::getIndicesCount() const {
        return mIndicesCount;
    }

} // vklite
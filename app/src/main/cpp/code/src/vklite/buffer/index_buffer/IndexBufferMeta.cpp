//
// Created by leixing on 2025/6/1.
//

#include "IndexBufferMeta.h"

namespace vklite {

    IndexBufferMeta::IndexBufferMeta(vk::IndexType indexType)
            : mIndexType(indexType) {}

    IndexBufferMeta::~IndexBufferMeta() = default;

    IndexBufferMeta::IndexBufferMeta(const IndexBufferMeta &other) = default;

    IndexBufferMeta &IndexBufferMeta::operator=(const IndexBufferMeta &other) {
        if (this != &other) {
            mIndexType = other.mIndexType;
        }
        return *this;
    }

    IndexBufferMeta::IndexBufferMeta(IndexBufferMeta &&other) noexcept
            : mIndexType(other.mIndexType) {}

    IndexBufferMeta &IndexBufferMeta::operator=(IndexBufferMeta &&other) noexcept {
        if (this != &other) {
            mIndexType = other.mIndexType;
        }
        return *this;
    }

    vk::IndexType IndexBufferMeta::getIndexType() const {
        return mIndexType;
    }

} // vklite
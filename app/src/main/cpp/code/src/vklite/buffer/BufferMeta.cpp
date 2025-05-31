//
// Created by leixing on 2025/5/31.
//

#include "BufferMeta.h"

namespace vklite {

    BufferMeta::BufferMeta(vk::DeviceSize size)
            : mSize(size) {}

    BufferMeta::~BufferMeta() = default;

    BufferMeta::BufferMeta(const BufferMeta &other)
            : mSize(other.mSize) {}

    BufferMeta &BufferMeta::operator=(const BufferMeta &other) {
        if (this != &other) {
            mSize = other.mSize;
        }
        return *this;
    }

    BufferMeta::BufferMeta(BufferMeta &&other) noexcept
            : mSize(other.mSize) {}

    BufferMeta &BufferMeta::operator=(BufferMeta &&other) noexcept {
        if (this != &other) {
            mSize = other.mSize;
        }
        return *this;
    }

    vk::DeviceSize BufferMeta::getSize() const {
        return mSize;
    }

} // vklite
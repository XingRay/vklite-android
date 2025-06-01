//
// Created by leixing on 2025/3/11.
//
#include "VertexAttributeConfigure.h"

namespace vklite {

    VertexAttributeConfigure::VertexAttributeConfigure(vk::Format format, uint32_t offset)
            : mFormat(format), mOffset(offset) {}

    VertexAttributeConfigure::~VertexAttributeConfigure() = default;

    VertexAttributeConfigure::VertexAttributeConfigure(const VertexAttributeConfigure &other) = default;

    VertexAttributeConfigure &VertexAttributeConfigure::operator=(const VertexAttributeConfigure &other) {
        if (this != &other) {
            mFormat = other.mFormat;
            mOffset = other.mOffset;
        }
        return *this;
    }

    VertexAttributeConfigure::VertexAttributeConfigure(VertexAttributeConfigure &&other) noexcept
            : mFormat(other.mFormat),
              mOffset(other.mOffset) {}

    VertexAttributeConfigure &VertexAttributeConfigure::operator=(VertexAttributeConfigure &&other) noexcept {
        if (this != &other) {
            mFormat = other.mFormat;
            mOffset = other.mOffset;
        }
        return *this;
    }

    vk::Format VertexAttributeConfigure::getFormat() const {
        return mFormat;
    }

    uint32_t VertexAttributeConfigure::getOffset() const {
        return mOffset;
    }
}
//
// Created by leixing on 2025/5/20.
//

#include "FrameBuffers.h"

namespace vklite {

    FrameBuffers::FrameBuffers()
            : mFrameBuffers{} {}

    FrameBuffers::FrameBuffers(std::vector<FrameBuffer> frameBuffers)
            : mFrameBuffers(std::move(frameBuffers)) {}

    FrameBuffers::~FrameBuffers() = default;

    FrameBuffers::FrameBuffers(FrameBuffers &&other) noexcept
            : mFrameBuffers(std::move(other.mFrameBuffers)) {}

    FrameBuffers &FrameBuffers::operator=(FrameBuffers &&other) noexcept {
        if (this != &other) {
            mFrameBuffers = std::move(other.mFrameBuffers);
        }
        return *this;
    }

    FrameBuffer &FrameBuffers::operator[](size_t index) {
        return mFrameBuffers[index];
    }

} // vklite
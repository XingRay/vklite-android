//
// Created by leixing on 2025/5/20.
//

#include "FrameBuffersBuilder.h"

namespace vklite {

    FrameBuffersBuilder::FrameBuffersBuilder() : mCount(0) {}

    FrameBuffersBuilder::~FrameBuffersBuilder() = default;

    FrameBuffersBuilder &FrameBuffersBuilder::count(uint32_t count) {
        mCount = count;
        return *this;
    }

    FrameBuffersBuilder &FrameBuffersBuilder::frameBufferBuilder(std::function<FrameBuffer(uint32_t index)> &&frameBufferBuilder) {
        mFrameBufferBuilder = std::move(frameBufferBuilder);
        return *this;
    }

    FrameBuffers FrameBuffersBuilder::build() {
        std::vector<FrameBuffer> frameBuffers;
        frameBuffers.reserve(mCount);
        for (uint32_t index = 0; index < mCount; index++) {
            frameBuffers.push_back(mFrameBufferBuilder(index));
        }
        return FrameBuffers(std::move(frameBuffers));
    }

    std::unique_ptr<FrameBuffers> FrameBuffersBuilder::buildUnique() {
        std::vector<FrameBuffer> frameBuffers;
        frameBuffers.reserve(mCount);
        for (uint32_t index = 0; index < mCount; index++) {
            frameBuffers.push_back(mFrameBufferBuilder(index));
        }
        return std::make_unique<FrameBuffers>(std::move(frameBuffers));
    }

} // vklite
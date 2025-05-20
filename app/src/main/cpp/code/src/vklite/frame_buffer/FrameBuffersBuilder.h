//
// Created by leixing on 2025/5/20.
//

#pragma once

#include <memory>
#include <cstdint>

#include "vklite/frame_buffer/FrameBuffers.h"

namespace vklite {

    class FrameBuffersBuilder {
    private:
        uint32_t mCount;
        std::function<FrameBuffer(uint32_t index)> mFrameBufferBuilder;

    public:
        FrameBuffersBuilder();

        ~FrameBuffersBuilder();

        FrameBuffersBuilder &count(uint32_t count);

        FrameBuffersBuilder &frameBufferBuilder(std::function<FrameBuffer(uint32_t index)> &&frameBufferBuilder);

        FrameBuffers build();

        std::unique_ptr<FrameBuffers> buildUnique();
    };

} // vklite

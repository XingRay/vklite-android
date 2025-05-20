//
// Created by leixing on 2025/5/20.
//

#pragma once

#include <vector>
#include <cstddef>

#include "vklite/frame_buffer/FrameBuffer.h"

namespace vklite {

    class FrameBuffers {
    private:
        std::vector<FrameBuffer> mFrameBuffers;

    public:
        FrameBuffers();

        explicit FrameBuffers(std::vector<FrameBuffer> frameBuffers);

        ~FrameBuffers();

        FrameBuffers(const FrameBuffers &other) = delete;

        FrameBuffers &operator=(const FrameBuffers &other) = delete;

        FrameBuffers(FrameBuffers &&other) noexcept;

        FrameBuffers &operator=(FrameBuffers &&other) noexcept;

        FrameBuffer &operator[](size_t index);
    };

} // vklite

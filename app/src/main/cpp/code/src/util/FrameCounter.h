//
// Created by leixing on 2025/5/21.
//

#pragma once

#include <chrono>
#include <cstdint>

namespace util {

    class FrameCounter {
    private:
        std::chrono::steady_clock::time_point mStartTime;

        uint32_t mFrameCount;

        // 重新计算fps的时间间隔, 单位 ms
        uint64_t mResetTimeMs;

        float mFps;
        bool mFpsUpdated;

    public:
        FrameCounter(uint64_t resetTimeMs);

        FrameCounter();

        ~FrameCounter();

        FrameCounter &start();

        FrameCounter &count();

        float getFps();

        bool getAndResetIsFpsUpdated();
    };

} // util

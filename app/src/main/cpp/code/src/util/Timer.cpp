//
// Created by leixing on 2025/5/26.
//

#include "Timer.h"

namespace util {

    Timer::Timer() = default;

    Timer::~Timer() = default;

    void Timer::start() {
        mStartTime = std::chrono::steady_clock::now();
    }

    uint32_t Timer::getDeltaTimeMs() {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - mStartTime).count();
        mStartTime = currentTime;
        return elapsedTime;
    }

} // util
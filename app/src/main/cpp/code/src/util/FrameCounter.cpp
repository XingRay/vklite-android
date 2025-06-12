//
// Created by leixing on 2025/5/21.
//

#include "FrameCounter.h"

namespace util {

    FrameCounter::FrameCounter(uint64_t resetTimeMs)
            : mResetTimeMs(resetTimeMs), mFrameCount(0), mFpsUpdated(false) {}

    FrameCounter::FrameCounter()
            : FrameCounter(5000) {}

    FrameCounter::~FrameCounter() = default;

    FrameCounter &FrameCounter::start() {
        mStartTime = std::chrono::steady_clock::now();
//        mStartTimeMs = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        return *this;
    }

    FrameCounter &FrameCounter::count() {
        mFrameCount++;

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - mStartTime).count();
        if (elapsedTime > mResetTimeMs) {
            mFps = static_cast<float>(mFrameCount * 1000) / static_cast<float>(elapsedTime);
            mStartTime = currentTime;
            mFrameCount = 0;
            mFpsUpdated = true;
        }
        return *this;
    }

    float FrameCounter::getFps() {
        return mFps;
    }

    bool FrameCounter::getAndResetIsFpsUpdated() {
        bool lastValue = mFpsUpdated;
        if (mFpsUpdated) {
            mFpsUpdated = false;
        }
        return lastValue;
    }

} // util
//
// Created by leixing on 2025/5/26.
//

#pragma once


#include <chrono>
#include <cstdint>

namespace util {

    class Timer {
    private:
        std::chrono::steady_clock::time_point mStartTime;

    public:
        Timer();

        ~Timer();

        void start();

        uint32_t getDeltaTimeMs();
    };

} // util

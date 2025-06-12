//
// Created by leixing on 2025/6/12.
//

#include "Range.h"

#include <format>

namespace ndkcamera {

    Range::Range(uint32_t min, uint32_t max)
            : mMin(min), mMax(max) {}

    Range::~Range() = default;

    Range::Range(const Range &other) = default;

    Range &Range::operator=(const Range &other) {
        if (this != &other) {
            mMin = other.mMin;
            mMax = other.mMax;
        }
        return *this;
    }

    Range::Range(Range &&other) noexcept = default;

    Range &Range::operator=(Range &&other) noexcept {
        if (this != &other) {
            mMin = other.mMin;
            mMax = other.mMax;
        }
        return *this;
    }

    [[nodiscard]]
    uint32_t Range::getMin() const {
        return mMin;
    }

    [[nodiscard]]
    uint32_t Range::getMax() const {
        return mMax;
    }

    [[nodiscard]]
    std::string Range::toString() const {
        return std::format("{{min:{},max:{}}}", mMin, mMax);
    }

} // ndkcamera
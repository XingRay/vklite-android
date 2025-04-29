//
// Created by leixing on 2025/4/29.
//

#include "MaxUint32Selector.h"

namespace vklite {

    MaxUint32Selector::MaxUint32Selector(uint32_t maxLimit)
            : mMaxLimit(maxLimit) {}

    MaxUint32Selector::MaxUint32Selector()
            : MaxUint32Selector(std::numeric_limits<uint32_t>::max()) {}

    MaxUint32Selector::~MaxUint32Selector() = default;

    uint32_t MaxUint32Selector::select(const std::vector<uint32_t> &candidates) const {
        bool found = false;
        uint32_t value = 0;

        for (uint32_t candidate: candidates) {
            if (candidate > value && candidate <= mMaxLimit) {
                value = candidate;
                found = true;
            }
        }

        if (!found) {
            throw std::runtime_error("value not found");
        }

        return value;
    }

} // vklite
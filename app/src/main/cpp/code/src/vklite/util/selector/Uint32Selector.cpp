//
// Created by leixing on 2025/1/11.
//

#include "Uint32Selector.h"

#include <stdexcept>

namespace vklite {
    FixUint32Selector::FixUint32Selector(uint32_t value) : mFixValue(value) {
    }

    FixUint32Selector::~FixUint32Selector() = default;

    uint32_t FixUint32Selector::select(const std::vector<uint32_t> &candidates) const {
        bool found = false;
        for (int i = 0; i < candidates.size(); i++) {
            if (candidates[i] == mFixValue) {
                found = true;
            }
        }
        if (!found) {
            throw std::runtime_error("value not found");
        }
        return mFixValue;
    }

    LambdaUint32Selector::LambdaUint32Selector(std::function<uint32_t(const std::vector<uint32_t> &)> selector)
            : mSelector(std::move(selector)) {
    }

    LambdaUint32Selector::~LambdaUint32Selector() = default;

    uint32_t LambdaUint32Selector::select(const std::vector<uint32_t> &candidates) const {
        return mSelector(candidates);
    }
};

// namespace vklite

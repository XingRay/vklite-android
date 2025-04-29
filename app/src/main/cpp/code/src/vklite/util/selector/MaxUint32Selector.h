//
// Created by leixing on 2025/4/29.
//

#pragma once

#include <vector>
#include <functional>

#include "vklite/util/selector/ValueSelector.h"

namespace vklite {

    class MaxUint32Selector : public ValueSelector<uint32_t> {
    private:
        uint32_t mMaxLimit;

    public:
        explicit MaxUint32Selector(uint32_t maxLimit);

        MaxUint32Selector();

        ~MaxUint32Selector() override;

        [[nodiscard]]
        uint32_t select(const std::vector<uint32_t> &candidates) const override;
    };

} // vklite

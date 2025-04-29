//
// Created by leixing on 2025/4/29.
//

#pragma once

#include <vector>

namespace vklite {

    template<typename T>
    class ValueSelector {
    public:
        virtual ~ValueSelector() = default;

        [[nodiscard]]
        virtual T select(const std::vector<T> &candidates) const = 0;
    };

} // vklite

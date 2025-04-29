//
// Created by leixing on 2025/4/29.
//

#pragma once
#include <vector>

namespace vklite {

    template<typename T>
    class ListSelector {
    public:
        virtual ~ListSelector() = default;

        [[nodiscard]]
        virtual std::vector<T> select(const std::vector<T> &candidates) const = 0;
    };

} // vklite

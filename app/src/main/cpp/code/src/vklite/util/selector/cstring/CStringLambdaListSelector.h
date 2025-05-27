//
// Created by leixing on 2025/1/8.
//

#pragma once

#include <vector>
#include <functional>

#include "vklite/util/selector/ListSelector.h"

namespace vklite {

    class CStringLambdaListSelector : public ListSelector<const char *> {
    private:
        std::function<std::vector<const char *>(const std::vector<const char *> &candidate)> mSelector;

    public:
        explicit CStringLambdaListSelector(std::function<std::vector<const char *>(const std::vector<const char *> &)> selector);

        ~CStringLambdaListSelector() override;

        [[nodiscard]]
        std::vector<const char *> select(const std::vector<const char *> &candidate) const override;
    };

} // common

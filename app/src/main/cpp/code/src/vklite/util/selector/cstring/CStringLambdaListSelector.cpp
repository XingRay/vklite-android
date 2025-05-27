//
// Created by leixing on 2025/1/8.
//

#include "CStringLambdaListSelector.h"

#include <stdexcept>
#include <utility>
#include "vklite/Log.h"

namespace vklite {

    CStringLambdaListSelector::CStringLambdaListSelector(std::function<std::vector<const char *>(const std::vector<const char *> &)> selector)
            : mSelector(std::move(selector)) {
    }

    CStringLambdaListSelector::~CStringLambdaListSelector() = default;

    std::vector<const char *> CStringLambdaListSelector::select(const std::vector<const char *> &candidate) const {
        return mSelector(candidate);
    }

} // common

//
// Created by leixing on 2025/5/28.
//

#include "CStringFixListSelector.h"

namespace vklite {

    CStringFixListSelector::CStringFixListSelector(const std::vector<const char *> &selected)
            : mSelected(selected) {}

    CStringFixListSelector::CStringFixListSelector(std::vector<const char *> &&selected)
            : mSelected(std::move(selected)) {}

    CStringFixListSelector::~CStringFixListSelector() = default;

    std::vector<const char *> CStringFixListSelector::select(const std::vector<const char *> &candidate) const {
        return mSelected;
    }

} // vklite
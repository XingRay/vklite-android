//
// Created by leixing on 2025/5/28.
//

#pragma once

#include <vector>
#include <functional>

#include "vklite/util/selector/ListSelector.h"

namespace vklite {

    class CStringFixListSelector : public ListSelector<const char *> {
    private:
        std::vector<const char *> mSelected;

    public:
        explicit CStringFixListSelector(const std::vector<const char *> &selected);

        explicit CStringFixListSelector(std::vector<const char *> &&selected);

        ~CStringFixListSelector() override;

        [[nodiscard]]
        std::vector<const char *> select(const std::vector<const char *> &candidate) const override;
    };

} // vklite

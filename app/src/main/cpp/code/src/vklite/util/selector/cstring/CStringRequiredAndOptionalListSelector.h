//
// Created by leixing on 2025/5/28.
//

#pragma once

#include <vector>
#include <functional>

#include "vklite/util/selector/ListSelector.h"

namespace vklite {

    class CStringRequiredAndOptionalListSelector : public ListSelector<const char *> {
    private:
        std::vector<const char *> mRequired;
        std::vector<const char *> mOptional;
    public:
        explicit CStringRequiredAndOptionalListSelector(std::vector<const char *> &&required,
                                                        std::vector<const char *> &&optional = {});

        ~CStringRequiredAndOptionalListSelector() override;

        [[nodiscard]]
        std::vector<const char *> select(const std::vector<const char *> &candidates) const override;
    };

} // vklite

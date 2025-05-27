//
// Created by leixing on 2025/5/28.
//

#include "CStringRequiredAndOptionalListSelector.h"

#include <format>
#include <assert.h>

#include "vklite/Log.h"

namespace vklite {

    CStringRequiredAndOptionalListSelector::CStringRequiredAndOptionalListSelector(std::vector<const char *> &&required,
                                                                                   std::vector<const char *> &&optional)
            : mRequired(std::move(required)), mOptional(std::move(optional)) {
    }

    CStringRequiredAndOptionalListSelector::~CStringRequiredAndOptionalListSelector() = default;

    std::vector<const char *> CStringRequiredAndOptionalListSelector::select(const std::vector<const char *> &candidates) const {
        std::vector<const char *> result;
        result.reserve(mRequired.size() + mOptional.size());

        // 自定义内容比较谓词
        auto compareCStr = [](const char *a, const char *b) {
            if (a == nullptr || b == nullptr) {
                return false;
            }
            return strcmp(a, b) == 0;
        };

        // 1. 处理必需项
        for (const char *requiredStr: mRequired) {
            assert(requiredStr != nullptr);

            auto it = std::find_if(candidates.begin(), candidates.end(),
                                   [&](const char *s) { return compareCStr(s, requiredStr); });

            if (it == candidates.end()) {
                throw std::runtime_error(std::format("Required string not found: {}", requiredStr));
            }

            // 内容去重
            bool exists = std::any_of(result.begin(), result.end(),
                                      [&](const char *s) { return compareCStr(s, requiredStr); });
            if (!exists) {
                result.push_back(*it);
            }
        }

        // 2. 处理可选项
        for (const char *optionalStr: mOptional) {
            assert(optionalStr != nullptr);

            auto it = std::find_if(candidates.begin(), candidates.end(),
                                   [&](const char *s) { return compareCStr(s, optionalStr); });

            if (it != candidates.end()) {
                result.push_back(*it);
            } else {
                LOG_WF("Optional string not found: {}\n", optionalStr);
            }
        }

        return result;
    }

} // vklite
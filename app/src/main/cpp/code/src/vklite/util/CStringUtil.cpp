//
// Created by leixing on 2025/5/27.
//

#include "CStringUtil.h"

#include <algorithm>
#include <cstring>

namespace vklite {

    bool compareCStr(const char *a, const char *b) {
        return std::strcmp(a, b) < 0; // 按字典序排序
    }

    bool equalCStr(const char *a, const char *b) {
        return std::strcmp(a, b) == 0; // 内容相同视为重复
    }

    std::vector<const char *> CStringUtil::removeDuplicates(std::vector<const char *> cstrings) {
        // 排序（改变原顺序）
        std::sort(cstrings.begin(), cstrings.end(), compareCStr);
        // 去重相邻元素
        auto last = std::unique(cstrings.begin(), cstrings.end(), equalCStr);
        cstrings.erase(last, cstrings.end());
        return cstrings;
    }

} // vklite
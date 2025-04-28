//
// Created by leixing on 2025/1/9.
//

#pragma once

#include <string>
#include <vector>

namespace vklite {

    class StringUtil {
    public:
        static std::vector<uint32_t> parseVersion(const std::string &version);

        static bool isContains(const std::vector<std::string> &src, const std::vector<std::string> &target);

        static std::vector<const char *> toStringPtrArray(const std::vector<std::string> &strings);
    };

} // common

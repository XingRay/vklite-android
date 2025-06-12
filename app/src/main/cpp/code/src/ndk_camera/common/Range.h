//
// Created by leixing on 2025/6/12.
//

#pragma once

#include <cstdint>
#include <string>

namespace ndkcamera {

    class Range {
    private:
        uint32_t mMin;
        uint32_t mMax;

    public:
        explicit Range(uint32_t min, uint32_t max);

        ~Range();

        Range(const Range &other);

        Range &operator=(const Range &other);

        Range(Range &&other) noexcept;

        Range &operator=(Range &&other) noexcept;

        [[nodiscard]]
        uint32_t getMin() const;

        [[nodiscard]]
        uint32_t getMax() const;

        [[nodiscard]]
        std::string toString() const;
    };

} // ndkcamera

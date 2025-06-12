//
// Created by leixing on 2025/6/12.
//

#pragma once

#include <cstdint>
#include <functional>

namespace ndkcamera {

    class HardwareLevel {
    public:
        enum class Value{
            //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_LIMITED                    = 0,
            LIMITED,

            //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_FULL                       = 1,
            FULL,

            //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY                     = 2,
            LEGACY,

            //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_3                          = 3,
            LEVEL_3,

            //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_EXTERNAL                   = 4,
            EXTERNAL,
        };

    private:
        const Value mValue;
        const char *mName;
        const uint32_t mRawEnumValue;

    private:
        explicit HardwareLevel(Value value, const char *name, uint32_t rawEnumValue);

    public:
        HardwareLevel(const HardwareLevel &other) = delete;

        HardwareLevel &operator=(const HardwareLevel &other) = delete;

        HardwareLevel(HardwareLevel &&other) = delete;

        HardwareLevel &operator=(HardwareLevel &&other) = delete;

        ~HardwareLevel();

        [[nodiscard]]
        const char *name() const;

        [[nodiscard]]
        Value value() const;

        [[nodiscard]]
        uint32_t rawEnumValue()const;

    public: //static
        static HardwareLevel LIMITED;
        static HardwareLevel FULL;
        static HardwareLevel LEGACY;
        static HardwareLevel LEVEL_3;
        static HardwareLevel EXTERNAL;

        static std::vector<std::reference_wrapper<const HardwareLevel>> instances();

        static std::vector<HardwareLevel::Value> values();

        static std::vector<const char *> names();


        static const HardwareLevel &of(Value value);

        static const HardwareLevel &of(const char *name);

        static const HardwareLevel &of(uint32_t rawEnumValue);
    };

} // ndkcamera

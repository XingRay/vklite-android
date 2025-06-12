//
// Created by leixing on 2025/6/12.
//

#pragma once

#include <cstdint>
#include <functional>

namespace ndkcamera {

    class CameraLensFacing {
    public:
        enum class Value {
            // acamera_metadata_enum_acamera_lens_facing#ACAMERA_LENS_FACING_FRONT= 0,
            FRONT,

            // acamera_metadata_enum_acamera_lens_facing#ACAMERA_LENS_FACING_BACK= 1,
            BACK,

            // acamera_metadata_enum_acamera_lens_facing#ACAMERA_LENS_FACING_EXTERNAL= 2,
            EXTERNAL,
        };

    private:
        const Value mValue;
        const char *mName;
        const uint32_t mRawEnumValue;

    private:
        explicit CameraLensFacing(Value value, const char *name, uint32_t rawEnumValue);

    public:
        CameraLensFacing(const CameraLensFacing &other) = delete;

        CameraLensFacing &operator=(const CameraLensFacing &other) = delete;

        CameraLensFacing(CameraLensFacing &&other) = delete;

        CameraLensFacing &operator=(CameraLensFacing &&other) = delete;

        ~CameraLensFacing();

        [[nodiscard]]
        const char *name() const;

        [[nodiscard]]
        Value value() const;

        [[nodiscard]]
        uint32_t rawEnumValue() const;

    public: //static
        static CameraLensFacing FRONT;
        static CameraLensFacing BACK;
        static CameraLensFacing EXTERNAL;


        static std::vector<std::reference_wrapper<const CameraLensFacing>> instances();

        static std::vector<CameraLensFacing::Value> values();

        static std::vector<const char *> names();


        static const CameraLensFacing &of(Value value);

        static const CameraLensFacing &of(const char *name);

        static const CameraLensFacing &of(uint32_t rawEnumValue);
    };

} // ndkcamera

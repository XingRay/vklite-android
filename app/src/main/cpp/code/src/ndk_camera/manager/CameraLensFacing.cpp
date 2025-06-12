//
// Created by leixing on 2025/6/12.
//

#include "CameraLensFacing.h"

#include <format>

#include <camera/NdkCameraMetadataTags.h>

namespace ndkcamera {

    CameraLensFacing::CameraLensFacing(const Value value, const char *name, uint32_t rawEnumValue)
            : mValue(value), mName(name), mRawEnumValue(rawEnumValue) {}

    CameraLensFacing::~CameraLensFacing() = default;

    const char *CameraLensFacing::name() const {
        return mName;
    }

    CameraLensFacing::Value CameraLensFacing::value() const {
        return mValue;
    }

    uint32_t CameraLensFacing::rawEnumValue() const {
        return mRawEnumValue;
    }

// 类外初始化静态成员
    CameraLensFacing CameraLensFacing::FRONT(CameraLensFacing::Value::FRONT, "FRONT", acamera_metadata_enum_acamera_lens_facing::ACAMERA_LENS_FACING_FRONT);
    CameraLensFacing CameraLensFacing::BACK(CameraLensFacing::Value::BACK, "BACK", acamera_metadata_enum_acamera_lens_facing::ACAMERA_LENS_FACING_BACK);
    CameraLensFacing CameraLensFacing::EXTERNAL(CameraLensFacing::Value::EXTERNAL, "EXTERNAL", acamera_metadata_enum_acamera_lens_facing::ACAMERA_LENS_FACING_EXTERNAL);


    std::vector<std::reference_wrapper<const CameraLensFacing>> CameraLensFacing::instances() {
        return {
                CameraLensFacing::FRONT,
                CameraLensFacing::BACK,
                CameraLensFacing::EXTERNAL,
        };
    }

    std::vector<CameraLensFacing::Value> CameraLensFacing::values() {
        std::vector<std::reference_wrapper<const CameraLensFacing>> instances = CameraLensFacing::instances();
        std::vector<CameraLensFacing::Value> values;
        values.reserve(instances.size());

        for (const CameraLensFacing &instance: instances) {
            values.push_back(instance.value());
        }

        return values;
    }

    std::vector<const char *> CameraLensFacing::names() {
        std::vector<std::reference_wrapper<const CameraLensFacing>> instances = CameraLensFacing::instances();
        std::vector<const char *> names;
        names.reserve(instances.size());

        for (const CameraLensFacing &instance: instances) {
            names.push_back(instance.name());
        }

        return names;
    }

    const CameraLensFacing &CameraLensFacing::of(Value value) {
        std::vector<std::reference_wrapper<const CameraLensFacing>> instances = CameraLensFacing::instances();
        for (const CameraLensFacing &instance: instances) {
            if (instance.value() == value) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for value: {}", (uint32_t) value));
    }

    const CameraLensFacing &CameraLensFacing::of(const char *name) {
        std::vector<std::reference_wrapper<const CameraLensFacing>> instances = CameraLensFacing::instances();
        for (const CameraLensFacing &instance: instances) {
            if (strcmp(instance.name(), name) == 0) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for name: {}", name));
    }

    const CameraLensFacing &CameraLensFacing::of(uint32_t rawEnumValue) {
        std::vector<std::reference_wrapper<const CameraLensFacing>> instances = CameraLensFacing::instances();
        for (const CameraLensFacing &instance: instances) {
            if (instance.rawEnumValue() == rawEnumValue) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for rawEnumValue: {}", rawEnumValue));
    }

} // ndkcamera
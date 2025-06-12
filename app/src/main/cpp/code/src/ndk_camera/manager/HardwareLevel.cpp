//
// Created by leixing on 2025/6/12.
//

#include "HardwareLevel.h"

#include <format>

#include <camera/NdkCameraMetadataTags.h>

namespace ndkcamera {

    HardwareLevel::HardwareLevel(const Value value, const char *name, uint32_t rawEnumValue)
            : mValue(value), mName(name), mRawEnumValue(rawEnumValue) {}

    HardwareLevel::~HardwareLevel() = default;

    const char *HardwareLevel::name() const {
        return mName;
    }

    HardwareLevel::Value HardwareLevel::value() const {
        return mValue;
    }

    uint32_t HardwareLevel::rawEnumValue() const {
        return mRawEnumValue;
    }

//    static HardwareLevel LIMITED;
//    static HardwareLevel FULL;
//    static HardwareLevel LEGACY;
//    static HardwareLevel LEVEL_3;
//    static HardwareLevel EXTERNAL;

// 类外初始化静态成员
    HardwareLevel HardwareLevel::LIMITED(HardwareLevel::Value::LIMITED, "LIMITED",
                                         acamera_metadata_enum_acamera_info_supported_hardware_level::ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_LIMITED);

    HardwareLevel HardwareLevel::FULL(HardwareLevel::Value::FULL, "FULL",
                                      acamera_metadata_enum_acamera_info_supported_hardware_level::ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_FULL);

    HardwareLevel HardwareLevel::LEGACY(HardwareLevel::Value::LEGACY, "LEGACY",
                                        acamera_metadata_enum_acamera_info_supported_hardware_level::ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY);

    HardwareLevel HardwareLevel::LEVEL_3(HardwareLevel::Value::LEVEL_3, "LEVEL_3",
                                         acamera_metadata_enum_acamera_info_supported_hardware_level::ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_3);

    HardwareLevel HardwareLevel::EXTERNAL(HardwareLevel::Value::EXTERNAL, "EXTERNAL",
                                          acamera_metadata_enum_acamera_info_supported_hardware_level::ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_EXTERNAL);

    std::vector<std::reference_wrapper<const HardwareLevel>> HardwareLevel::instances() {
        return {
                HardwareLevel::LIMITED,
                HardwareLevel::FULL,
                HardwareLevel::LEGACY,
                HardwareLevel::LEVEL_3,
                HardwareLevel::EXTERNAL,
        };
    }

    std::vector<HardwareLevel::Value> HardwareLevel::values() {
        std::vector<std::reference_wrapper<const HardwareLevel>> instances = HardwareLevel::instances();
        std::vector<HardwareLevel::Value> values;
        values.reserve(instances.size());

        for (const HardwareLevel &instance: instances) {
            values.push_back(instance.value());
        }

        return values;
    }

    std::vector<const char *> HardwareLevel::names() {
        std::vector<std::reference_wrapper<const HardwareLevel>> instances = HardwareLevel::instances();
        std::vector<const char *> names;
        names.reserve(instances.size());

        for (const HardwareLevel &instance: instances) {
            names.push_back(instance.name());
        }

        return names;
    }

    const HardwareLevel &HardwareLevel::of(Value value) {
        std::vector<std::reference_wrapper<const HardwareLevel>> instances = HardwareLevel::instances();
        for (const HardwareLevel &instance: instances) {
            if (instance.value() == value) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for value: {}", (uint32_t) value));
    }

    const HardwareLevel &HardwareLevel::of(const char *name) {
        std::vector<std::reference_wrapper<const HardwareLevel>> instances = HardwareLevel::instances();
        for (const HardwareLevel &instance: instances) {
            if (strcmp(instance.name(), name) == 0) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for name: {}", name));
    }

    const HardwareLevel &HardwareLevel::of(uint32_t rawEnumValue) {
        std::vector<std::reference_wrapper<const HardwareLevel>> instances = HardwareLevel::instances();
        for (const HardwareLevel &instance: instances) {
            if (instance.rawEnumValue() == rawEnumValue) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for rawEnumValue: {}", rawEnumValue));
    }

} // ndkcamera
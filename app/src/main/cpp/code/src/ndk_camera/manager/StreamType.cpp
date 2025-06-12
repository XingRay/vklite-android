//
// Created by leixing on 2025/6/12.
//

#include "StreamType.h"

#include <format>
#include <camera/NdkCameraMetadataTags.h>

namespace ndkcamera {

    StreamType::StreamType(const Value value, const char *name, uint32_t rawEnumValue)
            : mValue(value), mName(name), mRawEnumValue(rawEnumValue) {}

    StreamType::~StreamType() = default;

    const char *StreamType::name() const {
        return mName;
    }

    StreamType::Value StreamType::value() const {
        return mValue;
    }

    uint32_t StreamType::rawEnumValue() const {
        return mRawEnumValue;
    }

// 类外初始化静态成员
    StreamType StreamType::OUTPUT(StreamType::Value::OUTPUT, "OUTPUT",
                                  acamera_metadata_enum_acamera_scaler_available_stream_configurations::ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS_OUTPUT);
    StreamType StreamType::INPUT(StreamType::Value::INPUT, "INPUT",
                                 acamera_metadata_enum_acamera_scaler_available_stream_configurations::ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS_INPUT);

    std::vector<std::reference_wrapper<const StreamType>> StreamType::instances() {
        return {
                StreamType::OUTPUT,
                StreamType::INPUT,
        };
    }

    std::vector<StreamType::Value> StreamType::values() {
        std::vector<std::reference_wrapper<const StreamType>> instances = StreamType::instances();
        std::vector<StreamType::Value> values;
        values.reserve(instances.size());

        for (const StreamType &instance: instances) {
            values.push_back(instance.value());
        }

        return values;
    }

    std::vector<const char *> StreamType::names() {
        std::vector<std::reference_wrapper<const StreamType>> instances = StreamType::instances();
        std::vector<const char *> names;
        names.reserve(instances.size());

        for (const StreamType &instance: instances) {
            names.push_back(instance.name());
        }

        return names;
    }

    const StreamType &StreamType::of(Value value) {
        std::vector<std::reference_wrapper<const StreamType>> instances = StreamType::instances();
        for (const StreamType &instance: instances) {
            if (instance.value() == value) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for value: {}", (uint32_t) value));
    }

    const StreamType &StreamType::of(const char *name) {
        std::vector<std::reference_wrapper<const StreamType>> instances = StreamType::instances();
        for (const StreamType &instance: instances) {
            if (strcmp(instance.name(), name) == 0) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for name: {}", name));
    }

    const StreamType &StreamType::of(uint32_t rawEnumValue) {
        std::vector<std::reference_wrapper<const StreamType>> instances = StreamType::instances();
        for (const StreamType &instance: instances) {
            if (instance.rawEnumValue() == rawEnumValue) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for rawEnumValue: {}", rawEnumValue));
    }

} // ndkcamera
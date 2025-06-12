//
// Created by leixing on 2025/6/12.
//

#pragma once

#include <cstdint>
#include <functional>

namespace ndkcamera {

    class StreamType {
    public:
        enum class Value {
            OUTPUT,
            INPUT,
        };

    private:
        const Value mValue;
        const char *mName;
        const uint32_t mRawEnumValue;

    private:
        explicit StreamType(Value value, const char *name, uint32_t rawEnumValue);

    public:
        StreamType(const StreamType &other) = delete;

        StreamType &operator=(const StreamType &other) = delete;

        StreamType(StreamType &&other) = delete;

        StreamType &operator=(StreamType &&other) = delete;

        ~StreamType();

        [[nodiscard]]
        const char *name() const;

        [[nodiscard]]
        Value value() const;

        [[nodiscard]]
        uint32_t rawEnumValue()const;

    public: //static
        static StreamType OUTPUT;
        static StreamType INPUT;

        static std::vector<std::reference_wrapper<const StreamType>> instances();

        static std::vector<StreamType::Value> values();

        static std::vector<const char *> names();


        static const StreamType &of(Value value);

        static const StreamType &of(const char *name);

        static const StreamType &of(uint32_t rawEnumValue);
        
    };

} // ndkcamera

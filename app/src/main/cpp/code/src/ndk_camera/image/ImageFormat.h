//
// Created by leixing on 2025/6/11.
//

#pragma once

#include <vector>
#include <optional>
#include <functional>

namespace ndkcamera {
    class ImageFormat {
    public:
        enum class Value {
            RGBA_8888,
            RGBX_8888,
            RGB_888,
            RGB_565,
            RGBA_FP16,
            YUV_420_888,
            JPEG,
            RAW16,
            RAW_PRIVATE,
            RAW10,
            RAW12,
            DEPTH16,
            DEPTH_POINT_CLOUD,
            PRIVATE,
            Y8,
            HEIC,
            DEPTH_JPEG,
        };

    private:
        const Value mValue;
        const char *mName;
        const uint32_t mRawEnumValue;

    private:
        explicit ImageFormat(Value value, const char *name, uint32_t rawEnumValue);

    public:
        ImageFormat(const ImageFormat &other) = delete;

        ImageFormat &operator=(const ImageFormat &other) = delete;

        ImageFormat(ImageFormat &&other) = delete;

        ImageFormat &operator=(ImageFormat &&other) = delete;

        ~ImageFormat();

        [[nodiscard]]
        const char *name() const;

        [[nodiscard]]
        Value value() const;

        [[nodiscard]]
        uint32_t rawEnumValue()const;

    public: //static
        static ImageFormat RGBA_8888;
        static ImageFormat RGBX_8888;
        static ImageFormat RGB_888;
        static ImageFormat RGB_565;
        static ImageFormat RGBA_FP16;
        static ImageFormat YUV_420_888;
        static ImageFormat JPEG;
        static ImageFormat RAW16;
        static ImageFormat RAW_PRIVATE;
        static ImageFormat RAW10;
        static ImageFormat RAW12;
        static ImageFormat DEPTH16;
        static ImageFormat DEPTH_POINT_CLOUD;
        static ImageFormat PRIVATE;
        static ImageFormat Y8;
        static ImageFormat HEIC;
        static ImageFormat DEPTH_JPEG;

        static std::vector<std::reference_wrapper<const ImageFormat>> instances();

        static std::vector<ImageFormat::Value> values();

        static std::vector<const char *> names();


        static const ImageFormat &of(Value value);

        static const ImageFormat &of(const char *name);

        static const ImageFormat &of(uint32_t rawEnumValue);
    };
}

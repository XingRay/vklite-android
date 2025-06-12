//
// Created by leixing on 2025/6/11.
//

#include "ImageFormat.h"

#include <stdexcept>
#include <format>

#include <media/NdkImage.h>

namespace ndkcamera {

    ImageFormat::ImageFormat(const Value value, const char *name, uint32_t rawEnumValue)
            : mValue(value), mName(name), mRawEnumValue(rawEnumValue) {}

    ImageFormat::~ImageFormat() = default;

    const char *ImageFormat::name() const {
        return mName;
    }

    ImageFormat::Value ImageFormat::value() const {
        return mValue;
    }

    uint32_t ImageFormat::rawEnumValue() const {
        return mRawEnumValue;
    }

// 类外初始化静态成员
    ImageFormat ImageFormat::RGBA_8888(ImageFormat::Value::RGBA_8888, "RGBA_8888", AIMAGE_FORMATS::AIMAGE_FORMAT_RGBA_8888);
    ImageFormat ImageFormat::RGBX_8888(ImageFormat::Value::RGBX_8888, "RGBX_8888", AIMAGE_FORMATS::AIMAGE_FORMAT_RGBX_8888);
    ImageFormat ImageFormat::RGB_888(ImageFormat::Value::RGB_888, "RGB_888", AIMAGE_FORMATS::AIMAGE_FORMAT_RGB_888);
    ImageFormat ImageFormat::RGB_565(ImageFormat::Value::RGB_565, "RGB_565", AIMAGE_FORMATS::AIMAGE_FORMAT_RGB_565);
    ImageFormat ImageFormat::RGBA_FP16(ImageFormat::Value::RGBA_FP16, "RGBA_FP16", AIMAGE_FORMATS::AIMAGE_FORMAT_RGBA_FP16);
    ImageFormat ImageFormat::YUV_420_888(ImageFormat::Value::YUV_420_888, "YUV_420_888", AIMAGE_FORMATS::AIMAGE_FORMAT_YUV_420_888);
    ImageFormat ImageFormat::JPEG(ImageFormat::Value::JPEG, "JPEG", AIMAGE_FORMATS::AIMAGE_FORMAT_JPEG);
    ImageFormat ImageFormat::RAW16(ImageFormat::Value::RAW16, "RAW16", AIMAGE_FORMATS::AIMAGE_FORMAT_RAW16);
    ImageFormat ImageFormat::RAW_PRIVATE(ImageFormat::Value::RAW_PRIVATE, "RAW_PRIVATE", AIMAGE_FORMATS::AIMAGE_FORMAT_RAW_PRIVATE);
    ImageFormat ImageFormat::RAW10(ImageFormat::Value::RAW10, "RAW10", AIMAGE_FORMATS::AIMAGE_FORMAT_RAW10);
    ImageFormat ImageFormat::RAW12(ImageFormat::Value::RAW12, "RAW12", AIMAGE_FORMATS::AIMAGE_FORMAT_RAW12);
    ImageFormat ImageFormat::DEPTH16(ImageFormat::Value::DEPTH16, "DEPTH16", AIMAGE_FORMATS::AIMAGE_FORMAT_DEPTH16);
    ImageFormat ImageFormat::DEPTH_POINT_CLOUD(ImageFormat::Value::DEPTH_POINT_CLOUD, "DEPTH_POINT_CLOUD", AIMAGE_FORMATS::AIMAGE_FORMAT_DEPTH_POINT_CLOUD);
    ImageFormat ImageFormat::PRIVATE(ImageFormat::Value::PRIVATE, "PRIVATE", AIMAGE_FORMATS::AIMAGE_FORMAT_PRIVATE);
    ImageFormat ImageFormat::Y8(ImageFormat::Value::Y8, "Y8", AIMAGE_FORMATS::AIMAGE_FORMAT_Y8);
    ImageFormat ImageFormat::HEIC(ImageFormat::Value::HEIC, "HEIC", AIMAGE_FORMATS::AIMAGE_FORMAT_HEIC);
    ImageFormat ImageFormat::DEPTH_JPEG(ImageFormat::Value::DEPTH_JPEG, "DEPTH_JPEG", AIMAGE_FORMATS::AIMAGE_FORMAT_DEPTH_JPEG);

    std::vector<std::reference_wrapper<const ImageFormat>> ImageFormat::instances() {
        return {
                ImageFormat::RGBA_8888,
                ImageFormat::RGBX_8888,
                ImageFormat::RGB_888,
                ImageFormat::RGB_565,
                ImageFormat::RGBA_FP16,
                ImageFormat::YUV_420_888,
                ImageFormat::JPEG,
                ImageFormat::RAW16,
                ImageFormat::RAW_PRIVATE,
                ImageFormat::RAW10,
                ImageFormat::RAW12,
                ImageFormat::DEPTH16,
                ImageFormat::DEPTH_POINT_CLOUD,
                ImageFormat::PRIVATE,
                ImageFormat::Y8,
                ImageFormat::HEIC,
                ImageFormat::DEPTH_JPEG,
        };
    }

    std::vector<ImageFormat::Value> ImageFormat::values() {
        std::vector<std::reference_wrapper<const ImageFormat>> instances = ImageFormat::instances();
        std::vector<ImageFormat::Value> values;
        values.reserve(instances.size());

        for (const ImageFormat &instance: instances) {
            values.push_back(instance.value());
        }

        return values;
    }

    std::vector<const char *> ImageFormat::names() {
        std::vector<std::reference_wrapper<const ImageFormat>> instances = ImageFormat::instances();
        std::vector<const char *> names;
        names.reserve(instances.size());

        for (const ImageFormat &instance: instances) {
            names.push_back(instance.name());
        }

        return names;
    }

    const ImageFormat &ImageFormat::of(Value value) {
        std::vector<std::reference_wrapper<const ImageFormat>> instances = ImageFormat::instances();
        for (const ImageFormat &instance: instances) {
            if (instance.value() == value) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for value: {}", (uint32_t) value));
    }

    const ImageFormat &ImageFormat::of(const char *name) {
        std::vector<std::reference_wrapper<const ImageFormat>> instances = ImageFormat::instances();
        for (const ImageFormat &instance: instances) {
            if (strcmp(instance.name(), name) == 0) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for name: {}", name));
    }

    const ImageFormat &ImageFormat::of(uint32_t rawEnumValue) {
        std::vector<std::reference_wrapper<const ImageFormat>> instances = ImageFormat::instances();
        for (const ImageFormat &instance: instances) {
            if (instance.rawEnumValue() == rawEnumValue) {
                return instance;
            }
        }
        throw std::runtime_error(std::format("can not find instance for rawEnumValue: {}", rawEnumValue));
    }
}

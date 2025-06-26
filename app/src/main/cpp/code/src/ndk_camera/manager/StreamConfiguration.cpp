//
// Created by leixing on 2025/6/12.
//

#include "StreamConfiguration.h"
#include <format>

namespace ndkcamera {

    StreamConfiguration::StreamConfiguration(ImageFormat::Value imageFormat,
                                             int32_t width,
                                             int32_t height,
                                             StreamType::Value streamType)
            : mImageFormat(imageFormat), mWidth(width), mHeight(height), mStreamType(streamType) {}

    StreamConfiguration::~StreamConfiguration() = default;

    StreamConfiguration::StreamConfiguration(const StreamConfiguration &other) = default;

    StreamConfiguration &StreamConfiguration::operator=(const StreamConfiguration &other) {
        if (this != &other) {
            mImageFormat = other.mImageFormat;
            mWidth = other.mWidth;
            mHeight = other.mHeight;
            mStreamType = other.mStreamType;
        }
        return *this;
    }

    StreamConfiguration::StreamConfiguration(StreamConfiguration &&other) noexcept = default;

    StreamConfiguration &StreamConfiguration::operator=(StreamConfiguration &&other) noexcept {
        if (this != &other) {
            mImageFormat = other.mImageFormat;
            mWidth = other.mWidth;
            mHeight = other.mHeight;
            mStreamType = other.mStreamType;
        }
        return *this;
    }

    [[nodiscard]]
    ImageFormat::Value StreamConfiguration::getImageFormat() const {
        return mImageFormat;
    }

    [[nodiscard]]
    int32_t StreamConfiguration::getWidth() const {
        return mWidth;
    }

    [[nodiscard]]
    int32_t StreamConfiguration::getHeight() const {
        return mHeight;
    }

    [[nodiscard]]
    StreamType::Value StreamConfiguration::getStreamType() const {
        return mStreamType;
    }

    [[nodiscard]]
    std::string StreamConfiguration::toString() const {
        return std::format("{{ imageFormat:{}, width:{}, height:{}, streamType:{} }}",
                           ImageFormat::of(mImageFormat).name(),
                           mWidth,
                           mHeight,
                           StreamType::of(mStreamType).name());
    }

} // ndkcamera
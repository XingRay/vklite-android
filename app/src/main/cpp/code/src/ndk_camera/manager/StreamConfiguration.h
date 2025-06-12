//
// Created by leixing on 2025/6/12.
//

#pragma once

#include <string>

#include "ndk_camera/image/ImageFormat.h"
#include "ndk_camera/manager/StreamType.h"


namespace ndkcamera {

    class StreamConfiguration {
    private:
        ImageFormat::Value mImageFormat;
        int32_t mWidth;
        int32_t mHeight;
        StreamType::Value mStreamType;

    public:
        StreamConfiguration(ImageFormat::Value imageFormat,
                            int32_t width,
                            int32_t height,
                            StreamType::Value streamType);

        ~StreamConfiguration();

        StreamConfiguration(const StreamConfiguration &other);

        StreamConfiguration &operator=(const StreamConfiguration &other);

        StreamConfiguration(StreamConfiguration &&other) noexcept;

        StreamConfiguration &operator=(StreamConfiguration &&other) noexcept;

        [[nodiscard]]
        ImageFormat::Value getImageFormat() const;

        [[nodiscard]]
        int32_t getWidth() const;

        [[nodiscard]]
        int32_t getHeight() const;

        [[nodiscard]]
        StreamType::Value getStreamType() const;

        [[nodiscard]]
        std::string toString() const;
    };

} // ndkcamera

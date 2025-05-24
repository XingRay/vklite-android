//
// Created by leixing on 2025/1/21.
//

#pragma once

#include <media/NdkImage.h>
#include <__variant/monostate.h>

namespace ndkcamera {

    class Image {
    private:
        AImage *mImage;

    public:
        explicit Image(AImage *image);

        ~Image();

        Image(const Image &other) = delete;

        Image &operator=(const Image &other) = delete;

        Image(Image &&other) noexcept;

        Image &operator=(Image &&other) noexcept;


        [[nodiscard]]
        AImage *getImage() const;

        AHardwareBuffer *getHardwareBuffer();
    };

} // ndkcamera

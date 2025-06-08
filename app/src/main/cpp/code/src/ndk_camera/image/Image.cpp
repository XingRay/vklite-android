//
// Created by leixing on 2025/1/21.
//

#include "Image.h"

#include <utility>

#include "ndk_camera/Log.h"

namespace ndkcamera {

    Image::Image(AImage *image)
            : mImage(image) {}

    Image::~Image() {
        if (mImage != nullptr) {
            LOG_D("AImage_delete(mImage); mImage=>%p", mImage);
            AImage_delete(mImage);
        }
    }

    Image::Image(Image &&other) noexcept
            : mImage(std::exchange(other.mImage, nullptr)) {}

    Image &Image::operator=(Image &&other) noexcept {
        if (this != &other) {
            mImage = std::exchange(other.mImage, nullptr);
        }
        return *this;
    }

    AImage *Image::getImage() const {
        return mImage;
    }

    AHardwareBuffer *Image::getHardwareBuffer() {
        AHardwareBuffer *buffer = nullptr;
        media_status_t status = AImage_getHardwareBuffer(mImage, &buffer);
        if (status != AMEDIA_OK || buffer == nullptr) {
            LOG_E("Image::getHardwareBuffer(): status=>%d, buffer:%p", status, buffer);
            return nullptr;
        }
        return buffer;
    }

} // ndkcamera
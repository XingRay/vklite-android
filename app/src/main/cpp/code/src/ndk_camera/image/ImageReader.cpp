//
// Created by leixing on 2025/1/20.
//

#include "ImageReader.h"

#include <algorithm>
#include <vector>

#include "ndk_camera/Log.h"
#include "camera/NdkCameraDevice.h"

#include <utility>

namespace ndkcamera {

    ImageReader::ImageReader(AImageReader *imageReader)
            : mImageReader(imageReader) {}

    ImageReader::~ImageReader() {
        AImageReader_delete(mImageReader);
    }

    ImageReader::ImageReader(ImageReader &&other) noexcept
            : mImageReader(std::exchange(other.mImageReader, nullptr)) {}

    ImageReader &ImageReader::operator=(ImageReader &&other) noexcept {
        if (this != &other) {
            mImageReader = std::exchange(other.mImageReader, nullptr);
        }
        return *this;
    }

    NativeWindow ImageReader::getWindow() {
        ANativeWindow *nativeWindow;
        // mImageReaderWindow 由 mImageReader 自动管理, 不需要手动释放
        media_status_t status = AImageReader_getWindow(mImageReader, &nativeWindow);

        if (status != AMEDIA_OK || nativeWindow == nullptr) {
            LOG_E("AImageReader_getWindow(), Failed to get ANativeWindow from AImageReader, status:%d, nativeWindow:%p", status, nativeWindow);
            throw std::runtime_error("AImageReader_getWindow(), Failed to get ANativeWindow from AImageReader");
        }
        return NativeWindow(nativeWindow);
    }

    std::unique_ptr<NativeWindow> ImageReader::getUniqueWindow() {
        return std::make_unique<NativeWindow>(getWindow());
    }


    CaptureSessionOutput ImageReader::createCaptureSessionOutput() {
        NativeWindow nativeWindow = getWindow();

        ACaptureSessionOutput *sessionOutput;
        camera_status_t status = ACaptureSessionOutput_create(nativeWindow.getNativeWindow(), &sessionOutput);
        if (status != ACAMERA_OK || sessionOutput == nullptr) {
            LOG_E("ACaptureSessionOutput_create(), Failed to create CaptureSessionOutput from ImageReaderWindow, status:%d, sessionOutput:%p", status, sessionOutput);
            throw std::runtime_error("ACaptureSessionOutput_create(), Failed to create CaptureSessionOutput from ImageReaderWindow");
        }

        return CaptureSessionOutput(sessionOutput);
    }

    std::unique_ptr<CaptureSessionOutput> ImageReader::createUniqueCaptureSessionOutput() {
        return std::make_unique<CaptureSessionOutput>(createCaptureSessionOutput());
    }

    CameraOutputTarget ImageReader::createCameraOutputTarget() {
        NativeWindow nativeWindow = getWindow();

        ACameraOutputTarget *cameraOutputTarget;
        camera_status_t status = ACameraOutputTarget_create(nativeWindow.getNativeWindow(), &cameraOutputTarget);
        if (status != ACAMERA_OK || cameraOutputTarget == nullptr) {
            LOG_E("ACameraOutputTarget_create() Failed, status:%d, cameraOutputTarget:%p", status, cameraOutputTarget);
            throw std::runtime_error("ACameraOutputTarget_create() Failed");
        }

        return CameraOutputTarget(cameraOutputTarget);
    }

    std::unique_ptr<CameraOutputTarget> ImageReader::createUniqueCameraOutputTarget() {
        return std::make_unique<CameraOutputTarget>(createCameraOutputTarget());
    }

    void ImageReader::setImageListener(std::function<void(const ImageReader &reader)> &&imageListener) {
//        mImageListener = std::move(imageListener);
    }

    void ImageReader::onImageAvailable() {
        LOG_D("ImageReader::onImageAvailable, mImageReader:%p", mImageReader);
        if (mImageListener != nullptr) {
            mImageListener(*this);
        }
    }

    std::optional<Image> ImageReader::acquireLatestImage() {
        AImage *image = nullptr;
        media_status_t status = AImageReader_acquireLatestImage(mImageReader, &image);
        if (status != AMEDIA_OK || image == nullptr) {
            return std::nullopt;
        }
        return Image(image);
    }

    ImageReader ImageReader::build(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages) {
        if (maxImages < 2) {
            throw std::runtime_error("Max images must >= 2");
        }

        AImageReader *imageReader;
        media_status_t status = AImageReader_newWithUsage(width, height, format, usage, maxImages, &imageReader);
        if (status != AMEDIA_OK || imageReader == nullptr) {
            LOG_E("Failed to create AImageReader");
            throw std::runtime_error("Failed to create AImageReader");
        }
        return ImageReader(imageReader);
    }

    std::unique_ptr<ImageReader> ImageReader::buildUnique(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages) {
        return std::make_unique<ImageReader>(build(width, height, format, usage, maxImages));
    }


    void ImageReader::onImageAvailable(void *context, AImageReader *reader) {
        ImageReader *imageReader = static_cast<ImageReader *>(context);
        LOG_D("static ImageReader::onImageAvailable, reader:%p", reader);
        imageReader->onImageAvailable();
    }

} // ndkcamera
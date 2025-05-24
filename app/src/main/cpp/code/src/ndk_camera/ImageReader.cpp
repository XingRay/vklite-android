//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/ImageReader.h"

#include <algorithm>
#include <vector>

#include "ndk_camera/Log.h"
#include "camera/NdkCameraDevice.h"

#include <utility>

namespace ndkcamera {

//    ImageReader::ImageReader(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages)
//            : mWidth(width), mHeight(height), mFormat(format), mUsage(usage), mMaxImages(maxImages), mImageReader(nullptr), mImageReaderWindow(nullptr) {
//
//        if (maxImages < 2) {
//            throw std::runtime_error("Max images must >= 2");
//        }
//
//        media_status_t status = AImageReader_newWithUsage(width, height, format, usage, maxImages, &mImageReader);
//        if (status != AMEDIA_OK || mImageReader == nullptr) {
//            LOG_E("Failed to create AImageReader");
//            return;
//        }
//
//        mImages.resize(maxImages);
//        mBuffers.resize(maxImages);
//        mCurrentBufferIndex = 0;
//
////        AImageReader_ImageListener listener = {this, onImageAvailable};
////        AImageReader_setImageListener(mImageReader, &listener);
//    }

    ImageReader::ImageReader(AImageReader *imageReader)
            : mImageReader(imageReader) {}

    ImageReader::~ImageReader() {
//        for (AImage *image: mImages) {
//            if (image != nullptr) {
//                AImage_delete(image);
//            }
//        }
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

    std::optional<NativeWindow> ImageReader::getWindow() {
        ANativeWindow *nativeWindow;
        // mImageReaderWindow 由 mImageReader 自动管理, 不需要手动释放
        media_status_t status = AImageReader_getWindow(mImageReader, &nativeWindow);

        if (status != AMEDIA_OK || nativeWindow == nullptr) {
            LOG_E("AImageReader_getWindow(), Failed to get ANativeWindow from AImageReader, status:%d, nativeWindow:%p", status, nativeWindow);
            return std::nullopt;
        }
        return NativeWindow(nativeWindow);
    }

    std::unique_ptr<NativeWindow> ImageReader::getUniqueWindow() {
        std::optional<NativeWindow> nativeWindow = getWindow();
        if (!nativeWindow.has_value()) {
            return nullptr;
        }
        return std::make_unique<NativeWindow>(std::move(nativeWindow.value()));
    }


    std::optional<CaptureSessionOutput> ImageReader::createCaptureSessionOutput() {
        std::optional<NativeWindow> nativeWindow = getWindow();
        if (!nativeWindow.has_value()) {
            LOG_E("createCaptureSessionOutput(), getWindow returns null");
            return std::nullopt;
        }

        ACaptureSessionOutput *sessionOutput;
        camera_status_t status = ACaptureSessionOutput_create(nativeWindow->getNativeWindow(), &sessionOutput);
        if (status != ACAMERA_OK || sessionOutput == nullptr) {
            LOG_E("ACaptureSessionOutput_create(), Failed to create CaptureSessionOutput from ImageReaderWindow, status:%d, sessionOutput:%p", status, sessionOutput);
            return std::nullopt;
        }

        return CaptureSessionOutput(sessionOutput);
    }

    std::unique_ptr<CaptureSessionOutput> ImageReader::createUniqueCaptureSessionOutput() {
        std::optional<CaptureSessionOutput> captureSessionOutput = createCaptureSessionOutput();
        if (!captureSessionOutput.has_value()) {
            return nullptr;
        }
        return std::make_unique<CaptureSessionOutput>(std::move(captureSessionOutput.value()));
    }

    std::optional<CameraOutputTarget> ImageReader::createCameraOutputTarget() {
        std::optional<NativeWindow> nativeWindow = getWindow();
        if (!nativeWindow.has_value()) {
            LOG_E("createCaptureSessionOutput(), getWindow returns null");
            return std::nullopt;
        }

        ACameraOutputTarget *cameraOutputTarget;
        camera_status_t status = ACameraOutputTarget_create(nativeWindow->getNativeWindow(), &cameraOutputTarget);
        if (status != ACAMERA_OK || cameraOutputTarget == nullptr) {
            LOG_E("ACameraOutputTarget_create() Failed, status:%d, cameraOutputTarget:%p", status, cameraOutputTarget);
            return std::nullopt;
        }

        return CameraOutputTarget(cameraOutputTarget);
    }

    std::unique_ptr<CameraOutputTarget> ImageReader::createUniqueCameraOutputTarget() {
        std::optional<CameraOutputTarget> cameraOutputTarget = createCameraOutputTarget();
        if (cameraOutputTarget.has_value()) {
            return std::make_unique<CameraOutputTarget>(std::move(cameraOutputTarget.value()));
        } else {
            return nullptr;
        }
    }

    void ImageReader::setImageListener(std::function<void(const AImageReader *reader)> &&imageListener) {
//        mImageListener = std::move(imageListener);
    }

    void ImageReader::onImageAvailable(void *context, AImageReader *reader) {
        ImageReader *imageReader = static_cast<ImageReader *>(context);
        LOG_D("static ImageReader::onImageAvailable, reader:%p", reader);
        imageReader->onImageAvailable();
    }

    void ImageReader::onImageAvailable() {
        LOG_D("ImageReader::onImageAvailable, mImageReader:%p", mImageReader);
//        mImageListener(mImageReader);
    }

    AHardwareBuffer *ImageReader::getLatestHardwareBuffer() {
        AImage *image = nullptr;
        media_status_t status = AImageReader_acquireLatestImage(mImageReader, &image);
        if (status != AMEDIA_OK || image == nullptr) {
            return nullptr; // 获取图像失败，返回空指针
        }

        AHardwareBuffer *buffer = nullptr;
        status = AImage_getHardwareBuffer(image, &buffer);
        if (status != AMEDIA_OK || buffer == nullptr) {
            AImage_delete(image);                       // 获取硬件缓冲区失败，释放图像
            return nullptr;
        }

//        // 更新当前缓冲区
//        mCurrentBufferIndex = (mCurrentBufferIndex + 1) % mBuffers.size();
//        if (mImages[mCurrentBufferIndex]) {
//            AImage_delete(mImages[mCurrentBufferIndex]); // 释放旧的图像
//        }
//        mImages[mCurrentBufferIndex] = image;           // 保存新的图像
//        mBuffers[mCurrentBufferIndex] = buffer;         // 保存新的硬件缓冲区

        return buffer;
    }

    void ImageReader::cleanLatestHardwareBuffer() {
//        AImage *image = mImages[mCurrentBufferIndex];
//        if (image != nullptr) {
//            AImage_delete(image);
//            mImages[mCurrentBufferIndex] = nullptr;
//            mBuffers[mCurrentBufferIndex] = nullptr;
//        }
    }

    std::optional<Image> ImageReader::acquireLatestImage() {
        AImage *image = nullptr;
        media_status_t status = AImageReader_acquireLatestImage(mImageReader, &image);
        if (status != AMEDIA_OK || image == nullptr) {
            return std::nullopt;
        }
        return Image(image);
    }

    std::optional<ImageReader> ImageReader::build(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages) {
        if (maxImages < 2) {
            throw std::runtime_error("Max images must >= 2");
        }

        AImageReader *imageReader;
        media_status_t status = AImageReader_newWithUsage(width, height, format, usage, maxImages, &imageReader);
        if (status != AMEDIA_OK || imageReader == nullptr) {
            LOG_E("Failed to create AImageReader");
            return std::nullopt;
        }
        return ImageReader(imageReader);
    }

    std::unique_ptr<ImageReader> ImageReader::buildUnique(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages) {
        std::optional<ImageReader> imageReader = build(width, height, format, usage, maxImages);
        if (!imageReader.has_value()) {
            return nullptr;
        }
        return std::make_unique<ImageReader>(std::move(imageReader.value()));
    }

} // ndkcamera
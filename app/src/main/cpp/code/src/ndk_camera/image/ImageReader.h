//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>
#include <functional>

#include <media/NdkImageReader.h>

#include "ndk_camera/session/CaptureSessionOutput.h"
#include "ndk_camera/request/CameraOutputTarget.h"
#include "ndk_camera/image/Image.h"
#include "ndk_camera/window/NativeWindow.h"

namespace ndkcamera {

    class ImageReader {
    private:
        AImageReader *mImageReader;

        std::function<void(const ImageReader &reader)> mImageListener;

    public:
        explicit ImageReader(AImageReader *imageReader);

        ~ImageReader();

        ImageReader(const ImageReader &other) = delete;

        ImageReader &operator=(const ImageReader &other) = delete;

        ImageReader(ImageReader &&other) noexcept;

        ImageReader &operator=(ImageReader &&other) noexcept;


        std::optional<NativeWindow> getWindow();

        std::unique_ptr<NativeWindow> getUniqueWindow();


        std::optional<CaptureSessionOutput> createCaptureSessionOutput();

        std::unique_ptr<CaptureSessionOutput> createUniqueCaptureSessionOutput();


        std::optional<CameraOutputTarget> createCameraOutputTarget();

        std::unique_ptr<CameraOutputTarget> createUniqueCameraOutputTarget();


        void setImageListener(std::function<void(const ImageReader &reader)> &&imageListener);

        std::optional<Image> acquireLatestImage();

    private:

        void onImageAvailable();

        // static methods

    public://static
        static std::optional<ImageReader> build(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages);

        static std::unique_ptr<ImageReader> buildUnique(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages);


    private:// static

        static void onImageAvailable(void *context, AImageReader *reader);
    };

} // ndkcamera

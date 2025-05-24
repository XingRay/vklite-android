//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>
#include <functional>

#include <media/NdkImageReader.h>

#include "ndk_camera/CaptureSessionOutput.h"
#include "ndk_camera/CameraOutputTarget.h"
#include "ndk_camera/Image.h"
#include "ndk_camera/NativeWindow.h"

namespace ndkcamera {

    class ImageReader {
    private:
//        int32_t mWidth;
//
//        int32_t mHeight;
//
//        int32_t mFormat;
//
//        uint64_t mUsage;
//
//        int32_t mMaxImages;

        AImageReader *mImageReader;

//        std::function<void(const AImageReader *reader)> mImageListener;
//
//        uint32_t mCurrentBufferIndex;
//        std::vector<AHardwareBuffer *> mBuffers;
////        std::vector<std::unique_ptr<Image>> mImages;
//        std::vector<AImage *> mImages;

    public:
        ImageReader(AImageReader *imageReader);

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


        void setImageListener(std::function<void(const AImageReader *reader)> &&imageListener);

        AHardwareBuffer *getLatestHardwareBuffer();

        void cleanLatestHardwareBuffer();

        std::optional<Image> acquireLatestImage();

    private:
        static void onImageAvailable(void *context, AImageReader *reader);

        void onImageAvailable();


    public://static
        static std::optional<ImageReader> build(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages);

        static std::unique_ptr<ImageReader> buildUnique(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages);
    };

} // ndkcamera

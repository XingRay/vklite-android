//
// Created by leixing on 2025/1/19.
//

#pragma once

#include <memory>
#include <functional>
#include <optional>

#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraDevice.h>

#include <media/NdkImageReader.h>
#include <android/hardware_buffer.h>

#include "ndk_camera/manager/CameraManager.h"
#include "ndk_camera/device/CameraDevice.h"
#include "ndk_camera/session/CameraCaptureSession.h"
#include "ndk_camera/image/ImageReader.h"
#include "ndk_camera/request/CaptureRequest.h"
#include "ndk_camera/request/CameraOutputTarget.h"

namespace ndkcamera {

    class NdkCamera {
    private:
        std::unique_ptr<CameraManager> mCameraManager;
        std::unique_ptr<CameraDevice> mCameraDevice;
        std::unique_ptr<CameraCaptureSession> mCaptureSession;
        std::unique_ptr<ImageReader> mImageReader;
        std::unique_ptr<CaptureSessionOutputContainer> mCaptureSessionOutputContainer;
        std::unique_ptr<CaptureSessionOutput> mCaptureSessionOutput;

        std::unique_ptr<CaptureRequest> mCaptureRequest;
        std::unique_ptr<CameraOutputTarget> mCameraOutputTarget;

        std::function<void(NdkCamera *camera, AHardwareBuffer *hardwareBuffer)> mPreviewCallback;
    public:
        explicit NdkCamera();

        ~NdkCamera();

        NdkCamera(const NdkCamera &other) = delete;

        NdkCamera &operator=(const NdkCamera &other) = delete;

        NdkCamera(NdkCamera &&other) noexcept;

        NdkCamera &operator=(NdkCamera &&other) noexcept;

        void setPreviewCallback(std::function<void(NdkCamera *camera, AHardwareBuffer *hardwareBuffer)> &&previewCallback);

        void startPreview();

        void stopPreview();

        std::optional<Image> acquireLatestImage();

        Image acquireLatestImageWithBuffer();

    private:
        static void onImageAvailable(void *context, AImageReader *reader);
    };

} // android

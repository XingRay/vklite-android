//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>

#include "camera/NdkCameraDevice.h"

#include "ndk_camera/CameraCaptureSession.h"
#include "ndk_camera/CaptureRequest.h"
#include "ndk_camera/CameraDeviceStateCallbacks.h"

namespace ndkcamera {

    class CameraDevice {
    private:
        ACameraDevice *mCameraDevice;
        std::unique_ptr<CameraDeviceStateCallbacks> mStateCallbacks;

    public:
        CameraDevice(ACameraDevice *cameraDevice, std::unique_ptr<CameraDeviceStateCallbacks> stateCallbacks);

        ~CameraDevice();

        CameraDevice(const CameraDevice &other) = delete;

        CameraDevice &operator=(const CameraDevice &other) = delete;

        CameraDevice(CameraDevice &&other) noexcept;

        CameraDevice &operator=(CameraDevice &&other) noexcept;


        // createCaptureRequest
        std::optional<CaptureRequest> createCaptureRequest();

        std::unique_ptr<CaptureRequest> createUniqueCaptureRequest();


        // createCaptureSession
        std::optional<CameraCaptureSession> createCaptureSession(const CaptureSessionOutputContainer &captureSessionOutputContainer);

        std::unique_ptr<CameraCaptureSession> createUniqueCaptureSession(const CaptureSessionOutputContainer &captureSessionOutputContainer);


        // createCaptureSessionWithSessionParameters
        std::optional<CameraCaptureSession> createCaptureSessionWithSessionParameters(const CaptureSessionOutputContainer &captureSessionOutputContainer,
                                                                                      const CaptureRequest &captureRequest);

        std::unique_ptr<CameraCaptureSession> createUniqueCaptureSessionWithSessionParameters(const CaptureSessionOutputContainer &captureSessionOutputContainer,
                                                                                              const CaptureRequest &captureRequest);

    private:
        // state callbacks
        void onDisconnected();

        void onError(int error);



        // static methods

    public://static
        static std::unique_ptr<CameraDeviceStateCallbacks> createUniqueStateCallbacks();

    private://static

        static void onDisconnected(void *context, ACameraDevice *device);

        static void onError(void *context, ACameraDevice *device, int error);
    };

} // ndkcamera

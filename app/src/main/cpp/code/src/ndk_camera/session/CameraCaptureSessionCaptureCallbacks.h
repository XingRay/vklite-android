//
// Created by leixing on 2025/5/24.
//

#pragma once

#include "camera/NdkCameraCaptureSession.h"

namespace ndkcamera {

    class CameraCaptureSessionCaptureCallbacks {
    private:
        ACameraCaptureSession_captureCallbacks *mCaptureCallbacks;

    public:
        CameraCaptureSessionCaptureCallbacks();

        ~CameraCaptureSessionCaptureCallbacks();

        CameraCaptureSessionCaptureCallbacks(const CameraCaptureSessionCaptureCallbacks &other) = delete;

        CameraCaptureSessionCaptureCallbacks &operator=(const CameraCaptureSessionCaptureCallbacks &other) = delete;

        CameraCaptureSessionCaptureCallbacks(CameraCaptureSessionCaptureCallbacks &&other) noexcept;

        CameraCaptureSessionCaptureCallbacks &operator=(CameraCaptureSessionCaptureCallbacks &&other) noexcept;

        ACameraCaptureSession_captureCallbacks &getCaptureCallbacks();
    };

} // ndkcamera
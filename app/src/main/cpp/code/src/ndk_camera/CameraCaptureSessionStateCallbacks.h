//
// Created by leixing on 2025/5/24.
//

#pragma once

#include "camera/NdkCameraCaptureSession.h"

namespace ndkcamera {

    class CameraCaptureSessionStateCallbacks {
    private:
        ACameraCaptureSession_stateCallbacks *mStateCallbacks;

    public:
        CameraCaptureSessionStateCallbacks();

        ~CameraCaptureSessionStateCallbacks();

        CameraCaptureSessionStateCallbacks(const CameraCaptureSessionStateCallbacks &other) = delete;

        CameraCaptureSessionStateCallbacks &operator=(const CameraCaptureSessionStateCallbacks &other) = delete;

        CameraCaptureSessionStateCallbacks(CameraCaptureSessionStateCallbacks &&other) noexcept;

        CameraCaptureSessionStateCallbacks &operator=(CameraCaptureSessionStateCallbacks &&other) noexcept;

        ACameraCaptureSession_stateCallbacks &getStateCallbacks();
    };

} // ndkcamera

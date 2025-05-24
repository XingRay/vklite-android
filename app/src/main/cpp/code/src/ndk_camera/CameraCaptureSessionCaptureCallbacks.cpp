//
// Created by leixing on 2025/5/24.
//

#include "CameraCaptureSessionCaptureCallbacks.h"

#include <utility>

namespace ndkcamera {

    CameraCaptureSessionCaptureCallbacks::CameraCaptureSessionCaptureCallbacks() {
        mCaptureCallbacks = new ACameraCaptureSession_captureCallbacks();
    }

    CameraCaptureSessionCaptureCallbacks::~CameraCaptureSessionCaptureCallbacks() {
        delete mCaptureCallbacks;
    }

    CameraCaptureSessionCaptureCallbacks::CameraCaptureSessionCaptureCallbacks(CameraCaptureSessionCaptureCallbacks &&other) noexcept
            : mCaptureCallbacks(std::exchange(other.mCaptureCallbacks, nullptr)) {}

    CameraCaptureSessionCaptureCallbacks &CameraCaptureSessionCaptureCallbacks::operator=(CameraCaptureSessionCaptureCallbacks &&other) noexcept {
        if (this != &other) {
            mCaptureCallbacks = std::exchange(other.mCaptureCallbacks, nullptr);
        }
        return *this;
    }

    ACameraCaptureSession_captureCallbacks &CameraCaptureSessionCaptureCallbacks::getCaptureCallbacks() {
        return *mCaptureCallbacks;
    }

} // ndkcamera
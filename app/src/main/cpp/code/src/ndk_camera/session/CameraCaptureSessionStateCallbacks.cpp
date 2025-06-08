//
// Created by leixing on 2025/5/24.
//

#include "CameraCaptureSessionStateCallbacks.h"

#include <utility>

namespace ndkcamera {

    CameraCaptureSessionStateCallbacks::CameraCaptureSessionStateCallbacks() {
        mStateCallbacks = new ACameraCaptureSession_stateCallbacks();
    }

    CameraCaptureSessionStateCallbacks::~CameraCaptureSessionStateCallbacks() {
        delete mStateCallbacks;
    }

    CameraCaptureSessionStateCallbacks::CameraCaptureSessionStateCallbacks(CameraCaptureSessionStateCallbacks &&other) noexcept
            : mStateCallbacks(std::exchange(other.mStateCallbacks, nullptr)) {}

    CameraCaptureSessionStateCallbacks &CameraCaptureSessionStateCallbacks::operator=(CameraCaptureSessionStateCallbacks &&other) noexcept {
        if (this != &other) {
            mStateCallbacks = std::exchange(other.mStateCallbacks, nullptr);
        }
        return *this;
    }

    ACameraCaptureSession_stateCallbacks &CameraCaptureSessionStateCallbacks::getStateCallbacks() {
        return *mStateCallbacks;
    }

} // ndkcamera
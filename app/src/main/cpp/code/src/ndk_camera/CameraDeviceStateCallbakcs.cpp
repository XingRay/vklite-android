//
// Created by leixing on 2025/5/24.
//

#include "CameraDeviceStateCallbacks.h"

#include <utility>

namespace ndkcamera {

    CameraDeviceStateCallbacks::CameraDeviceStateCallbacks() {
        mStateCallbacks = new ACameraDevice_StateCallbacks();
    }

    CameraDeviceStateCallbacks::~CameraDeviceStateCallbacks() {
        delete mStateCallbacks;
    }

    CameraDeviceStateCallbacks::CameraDeviceStateCallbacks(CameraDeviceStateCallbacks &&other) noexcept
            : mStateCallbacks(std::exchange(other.mStateCallbacks, nullptr)) {}

    CameraDeviceStateCallbacks &CameraDeviceStateCallbacks::operator=(CameraDeviceStateCallbacks &&other) noexcept {
        if (this != &other) {
            mStateCallbacks = std::exchange(other.mStateCallbacks, nullptr);
        }
        return *this;
    }

    ACameraDevice_StateCallbacks &CameraDeviceStateCallbacks::getStateCallbacks() {
        return *mStateCallbacks;
    }

} // ndkcamera
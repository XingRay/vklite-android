//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CameraOutputTarget.h"

#include <utility>

namespace ndkcamera {
    CameraOutputTarget::CameraOutputTarget(ACameraOutputTarget *outputTarget)
            : mCameraOutputTarget(outputTarget) {}

    CameraOutputTarget::~CameraOutputTarget() {
        ACameraOutputTarget_free(mCameraOutputTarget);
    }

    CameraOutputTarget::CameraOutputTarget(CameraOutputTarget &&other) noexcept
            : mCameraOutputTarget(std::exchange(other.mCameraOutputTarget, nullptr)) {}

    CameraOutputTarget &CameraOutputTarget::operator=(CameraOutputTarget &&other) noexcept {
        if (this != &other) {
            mCameraOutputTarget = std::exchange(other.mCameraOutputTarget, nullptr);
        }
        return *this;
    }

    ACameraOutputTarget *CameraOutputTarget::getCameraOutputTarget() const {
        return mCameraOutputTarget;
    }

} // ndkcamera
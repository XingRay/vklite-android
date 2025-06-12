//
// Created by leixing on 2025/1/20.
//

#pragma once

#include "camera/NdkCaptureRequest.h"

namespace ndkcamera {

    class CameraOutputTarget {
    private:
        ACameraOutputTarget *mCameraOutputTarget;

    public:
        explicit CameraOutputTarget(ACameraOutputTarget *outputTarget);

        ~CameraOutputTarget();

        CameraOutputTarget(const CameraOutputTarget &other) = delete;

        CameraOutputTarget &operator=(const CameraOutputTarget &other) = delete;

        CameraOutputTarget(CameraOutputTarget &&other) noexcept;

        CameraOutputTarget &operator=(CameraOutputTarget &&other) noexcept;


        [[nodiscard]]
        ACameraOutputTarget *getCameraOutputTarget() const;
    };

} // ndkcamera

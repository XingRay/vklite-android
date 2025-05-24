//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>

#include "camera/NdkCaptureRequest.h"
#include "ndk_camera/CameraOutputTarget.h"

namespace ndkcamera {

    class CaptureRequest {
    private:
        ACaptureRequest *mCaptureRequest;

    public:
        explicit CaptureRequest(ACaptureRequest *captureRequest);

        ~CaptureRequest();

        CaptureRequest(const CaptureRequest &other) = delete;

        CaptureRequest &operator=(const CaptureRequest &other) = delete;

        CaptureRequest(CaptureRequest &&other) noexcept;

        CaptureRequest &operator=(CaptureRequest &&other) noexcept;


        ACaptureRequest *getCaptureRequest() const;

        void addTarget(const std::unique_ptr<CameraOutputTarget> &cameraOutputTarget);

        camera_status_t setFps(int32_t fps);

        camera_status_t setFps(int32_t min, int32_t max);
    };

} // ndkcamera

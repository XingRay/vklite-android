//
// Created by leixing on 2025/1/20.
//

#include "CaptureRequest.h"
#include "ndk_camera/Log.h"

#include <utility>
#include <stdexcept>
#include <format>

namespace ndkcamera {
    CaptureRequest::CaptureRequest(ACaptureRequest *captureRequest) : mCaptureRequest(captureRequest) {

    }

    CaptureRequest::~CaptureRequest() {
        LOG_D("ACaptureRequest_free(captureRequest:%p)", mCaptureRequest);
        ACaptureRequest_free(mCaptureRequest);
    }

    CaptureRequest::CaptureRequest(CaptureRequest &&other) noexcept
            : mCaptureRequest(std::exchange(other.mCaptureRequest, nullptr)) {}

    CaptureRequest &CaptureRequest::operator=(CaptureRequest &&other) noexcept {
        if (this != &other) {
            mCaptureRequest = std::exchange(other.mCaptureRequest, nullptr);
        }
        return *this;
    }


    ACaptureRequest *CaptureRequest::getCaptureRequest() const {
        return mCaptureRequest;
    }

    void CaptureRequest::addTarget(const std::unique_ptr<CameraOutputTarget> &cameraOutputTarget) {
        LOG_D("CaptureRequest::addTarget()");
        LOG_D("ACaptureRequest_addTarget(mCaptureRequest:%p, cameraOutputTarget->getCameraOutputTarget():%p)", mCaptureRequest, cameraOutputTarget->getCameraOutputTarget());
        camera_status_t status = ACaptureRequest_addTarget(mCaptureRequest, cameraOutputTarget->getCameraOutputTarget());
        if (status != ACAMERA_OK) {
            LOG_D("CaptureRequest::addTarget(): ACaptureRequest_addTarget => %d", status);
            throw std::runtime_error("ACaptureRequest_addTarget failed");
        }
    }

    camera_status_t CaptureRequest::setFps(int32_t fps) {
        return setFps(fps, fps);
    }

    camera_status_t CaptureRequest::setFps(int32_t min, int32_t max) {
        // 设置帧率范围
        int32_t targetFpsRange[2] = {min, max}; // 设置目标帧率范围为 30 FPS
        camera_status_t status = ACaptureRequest_setEntry_i32(
                mCaptureRequest,
                ACAMERA_CONTROL_AE_TARGET_FPS_RANGE, // 控制帧率的参数
                2,                                   // 数组长度
                targetFpsRange                       // 帧率范围数组
        );

        if (status != ACAMERA_OK) {
            LOG_E("Failed to set target FPS range: %d", status);
        } else {
            LOG_D("Successfully set target FPS range to [%d, %d]", targetFpsRange[0], targetFpsRange[1]);
        }
        return status;
    }

} // ndkcamera